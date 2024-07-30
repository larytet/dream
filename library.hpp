#include <unordered_map>
#include <atomic>
#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <chrono>
#include <iostream>

#include <string>

class Book
{
public:
    std::string title;
    std::string author;
    std::string isbn;
    int publishedYear;
    int borrowedCount;

    Book() {}
    Book(std::string t, std::string a, std::string i, int y)
        : title(t), author(a), isbn(i), publishedYear(y) {}
};

class BookLibrary
{
private:
    std::unordered_map<std::string, bool> borrowedBooks;
    std::unordered_map<std::string, Book> books;
    std::mutex libraryMutex;
    std::condition_variable cv;
    size_t capacity;
    std::atomic<bool> stopFlag; // Atomic flag to signal stopping

    void sweep()
    {
        while (!stopFlag)
        {
            std::unique_lock<std::mutex> lock(libraryMutex);
            // Wait for the condition to be true or timeout
            if (cv.wait_for(lock, std::chrono::seconds(1), [this]
                            { return books.size() > capacity; }))
            {
                // Condition met, remove least popular book
            }
        }
    }

public:
    BookLibrary(size_t cap) : capacity(cap), stopFlag(false)
    {
        std::thread(&BookLibrary::sweep, this).detach();
    }

    void stopSweep()
    {
        // Signal the thread to stop
        stopFlag = true;
    }

    void addBook(const Book &book)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        auto isbn = book.isbn;
        books[isbn] = book;
        borrowedBooks[isbn] = false;
    }

    void removeBook(const std::string &isbn)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        books.erase(isbn);
        borrowedBooks.erase(isbn);
    }

    Book *lookupByTitle(const std::string &title)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        for (const auto &pair : books)
        {
            if (pair.second.title == title)
            {
                return &books[pair.first];
            }
        }
        return nullptr;
    }

    Book *lookupByIsbn(const std::string &isbn)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        if (books.find(isbn) != books.end())
        {
            return &books[isbn];
        }
        return nullptr;
    }

    bool borrowBook(const std::string &isbn)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        if (books.find(isbn) != books.end() && !borrowedBooks[isbn])
        {
            borrowedBooks[isbn] = true;
            books[isbn].borrowedCount++;
            return true;
        }
        return false;
    }

    void returnBook(const std::string &isbn)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        borrowedBooks[isbn] = false;
    }

    void statusReport()
    {
        std::lock_guard<std::mutex> lock(libraryMutex);
        size_t borrowedCount = 0;
        for (const auto &pair : borrowedBooks)
        {
            if (pair.second)
            {
                borrowedCount++;
            }
        }
        std::cout << "Total books: " << books.size() << std::endl;
        std::cout << "Borrowed books: " << borrowedCount << std::endl;
        std::cout << "Available books: " << books.size() - borrowedCount << std::endl;
    }
};
