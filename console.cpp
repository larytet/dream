#include <iostream>
#include <thread>
#include "library.hpp"

void addBook(BookLibrary &library)
{
    std::string title, author, isbn;
    int year;

    std::cout << "Enter book title: ";
    std::getline(std::cin, title);
    std::cout << "Enter book author: ";
    std::getline(std::cin, author);
    std::cout << "Enter book ISBN: ";
    std::getline(std::cin, isbn);
    std::cout << "Enter book publication year: ";
    std::cin >> year;
    std::cin.ignore(); // Ignore remaining newline character

    auto res = library.addBook(Book(title, author, isbn, year));
    if (res == Result::Value::BookAdded)
    {
        std::cout << "Book ISBN=" << isbn << " added successfully" << std::endl;
    }
    else
    {
        std::cout << "Error ISBN=" << isbn << " " << res << std::endl;
    }
}

void borrowBook(BookLibrary &library)
{
    std::string isbn;
    std::cout << "Enter ISBN of the book to borrow: ";
    std::getline(std::cin, isbn);

    auto res = library.borrowBook(isbn);
    if (res == Result::Value::BookBorrowed)
    {
        std::cout << "Borrowed book: ISBN=" << isbn << std::endl;
        return;
    }
    else
    {
        std::cout << "Error ISBN=" << isbn << " " << res << std::endl;
    }
}

void returnBook(BookLibrary &library)
{
    std::string isbn;
    std::cout << "Enter ISBN of the book to return: ";
    std::getline(std::cin, isbn);

    auto res = library.returnBook(isbn);
    if (res == Result::Value::BookReturned)
    {
        std::cout << "Returned book: ISBN=" << isbn << std::endl;
    }
    else
    {
        std::cout << "Error ISBN=" << isbn << " " << res << std::endl;
    }
}

void findBook(BookLibrary &library)
{
    std::string key;
    std::cout << "Enter ISBN or title of the book: ";
    std::getline(std::cin, key);

    Book book;
    bool ok = library.lookupByIsbn(key, &book);
    if (!ok)
    {
        ok = library.lookupByTitle(key, &book);
    }

    if (!ok)
    {
        std::cout << "Book not found: " << key << std::endl;
        return;
    }

    std::cout << "Book found: ISBN " << book.isbn << ", Title " << book.title << std::endl;
}

void printMenu()
{
    std::cout << std::endl
              << "Menu:" << std::endl;
    std::cout << "1. Add Book" << std::endl;
    std::cout << "2. Borrow Book" << std::endl;
    std::cout << "3. Return Book" << std::endl;
    std::cout << "4. Report" << std::endl;
    std::cout << "5. Find" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main()
{
    BookLibrary library(3);

    while (true)
    {
        printMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Ignore remaining newline character

        switch (choice)
        {
        case 1:
            addBook(library);
            break;
        case 2:
            borrowBook(library);
            break;
        case 3:
            returnBook(library);
            break;
        case 4:
            library.statusReport();
            break;
        case 5:
            findBook(library);
            break;
        case 6:
            std::cout << "Exiting..." << std::endl;
            library.stopSweep();
            return 0;
        default:
            std::cout << "Invalid choice. Please try again" << std::endl;
        }
    }

    return 0;
}
