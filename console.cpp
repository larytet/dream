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
    std::cout << "Book added successfully.\n";
}

void subscribeToLibrary(BookLibrary &library)
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
            std::cout << "Failed to borrow the book (maybe it's already borrowed).\n";
        }
    }
    else
    {
        std::cout << "No book found with the given ISBN.\n";
    }
}

void printMenu()
{
    std::cout << "\nMenu:\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. Subscribe (Borrow Book)\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

int main()
{
    BookLibrary library(10);

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
            subscribeToLibrary(library);
            break;
        case 3:
            std::cout << "Exiting...\n";
            library.stopBookSweep();
            return 0;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }

        // Print the status report after each action
        library.statusReport();
    }

    return 0;
}
