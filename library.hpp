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

#include "lruCache.hpp"

class Book
{
public:
    std::string title;
    std::string author;
    std::string isbn;
    int publishedYear;

    Book() {}
    Book(std::string t, std::string a, std::string i, int y)
        : title(t), author(a), isbn(i), publishedYear(y) {}
};

namespace Result
{
    enum class Value
    {
        TitleAlreadyExists,
        ISBNAlreadyExists,
        BookAdded,
        BookBorrowed,
        BookAlreadyBorrowed,
        BookNotBorrowed,
        BookReturned,
        ISBNMissing,
    };

    // Convert Result enum to string
    std::string ToString(Value value)
    {
        switch (value)
        {
        case Value::TitleAlreadyExists:
            return "Title Already exists";
        case Value::ISBNAlreadyExists:
            return "ISBN already exists";
        case Value::BookAdded:
            return "Book added";
        case Value::BookBorrowed:
            return "Book borrowed";
        case Value::BookAlreadyBorrowed:
            return "Book already borrowed";
        case Value::BookNotBorrowed:
            return "Book not borrowed";
        case Value::BookReturned:
            return "Book returned";
        case Value::ISBNMissing:
            return "ISBN missing";
        default:
            return "Unknown result " + std::to_string(static_cast<int>(value));
        }
    }

    // Overload the << operator to output Result directly to std::ostream
    std::ostream& operator<<(std::ostream& os, const Value& value)
    {
        os << ToString(value);
        return os;
    }
}


class BookLibrary
{
private:
    std::unordered_map<std::string, bool> borrowedBooks;
    std::unordered_map<std::string, Book> books;
    std::mutex libraryMutex;
    std::condition_variable cv;
    size_t capacity;
    std::atomic<bool> stopFlag; // Atomic flag to signal stopping
    LRUCache<std::string> lruCache;

    void sweep()
    {
        while (!stopFlag)
        {
            std::unique_lock<std::mutex> lock(libraryMutex);
            // Wait for the condition to be true or timeout
            if (cv.wait_for(lock, std::chrono::seconds(1), [this]
                            { return books.size() > capacity; }))
            {
                removeLeastPopularBooks();
            }
        }
    }

    void removeLeastPopularBooks()
    {
        while (!books.empty() && books.size() > capacity)
        {
            auto leastPopularBookISBN = lruCache.GetLeastUsed();
            removeBook(leastPopularBookISBN);
        }
    }

    void removeBook(const std::string &isbn)
    {
        books.erase(isbn);
        borrowedBooks.erase(isbn);
        lruCache.Remove(isbn);
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

    Result::Value addBook(const Book &book)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);

        auto isbn = book.isbn;
        // Check if the book with the same ISBN already exists
        if (books.find(isbn) != books.end())
        {
            std::cout << "A book with the " << isbn << " already exists." << std::endl;
            return Result::Value::ISBNAlreadyExists;
        }

        // Check if a book with the same title already exists (assuming unique titles for simplicity)
        for (const auto &[key, existingBook] : books)
        {
            if (existingBook.title == book.title)
            {
                std::cout << "A book with the title " << book.title << " already exists." << std::endl;
                return Result::Value::TitleAlreadyExists;
            }
        }

        // Add the book to the library
        books[isbn] = book;
        borrowedBooks[isbn] = false;
        lruCache.Add(isbn);

        // Trigger the sweep
        cv.notify_one();

        return Result::Value::BookAdded;
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

    Result::Value borrowBook(const std::string &isbn)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);

        if (books.find(isbn) == books.end())
        {
            return Result::Value::ISBNMissing;
        }

        if (borrowedBooks[isbn])
        {
            return Result::Value::BookAlreadyBorrowed;
        }

        borrowedBooks[isbn] = true;
        lruCache.Increment(isbn);

        return Result::Value::BookBorrowed;
    }

    Result::Value returnBook(const std::string &isbn)
    {
        std::lock_guard<std::mutex> lock(libraryMutex);

        if (books.find(isbn) == books.end())
        {
            return Result::Value::ISBNMissing;
        }

        if (!borrowedBooks[isbn])
        {
            return Result::Value::BookNotBorrowed;
        }
        borrowedBooks[isbn] = false;

        return Result::Value::BookReturned;
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
