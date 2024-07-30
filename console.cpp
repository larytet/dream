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

    library.addBook(Book(title, author, isbn, year));
    std::cout << "Book added successfully" << std::endl;
}

void borrowBook(BookLibrary &library)
{
    std::string isbn;
    std::cout << "Enter ISBN of the book to subscribe: ";
    std::getline(std::cin, isbn);

    Book *book = library.lookupByIsbn(isbn);
    if (book)
    {
        std::cout << "Found book: " << book->title << " by " << book->author << std::endl;
        if (library.borrowBook(isbn))
        {
            std::cout << "Borrowed book: " << book->title << std::endl;
        }
        else
        {
            std::cout << "Failed to borrow the book (maybe it's already borrowed)." << std::endl;
        }
    }
    else
    {
        std::cout << "No book found with the given ISBN." << std::endl;
    }
}

void printMenu()
{
    std::cout << std::endl
              << "Menu:" << std::endl;
    std::cout << "1. Add Book" << std::endl;
    std::cout << "2. Subscribe (Borrow Book)" << std::endl;
    std::cout << "3. Report" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main()
{
    BookLibrary library(1);

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
            library.statusReport();
            break;
        case 4:
            std::cout << "Exiting..." << std::endl;
            library.stopSweep();
            return 0;
        default:
            std::cout << "Invalid choice. Please try again" << std::endl;
        }
    }

    return 0;
}
