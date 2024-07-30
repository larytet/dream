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
    std::cout << "Enter ISBN of the book to borrow: ";
    std::getline(std::cin, isbn);

    Book *book = library.lookupByIsbn(isbn);
    if (!book)
    {
        std::cout << "No book found with the given ISBN." << std::endl;
        return;
    }
    if (!library.borrowBook(isbn))
    {
        std::cout << "Failed to borrow the book (maybe it's already borrowed)." << std::endl;
        return;
    }
    std::cout << "Borrowed book: " << book->title << std::endl;
}

void returnBook(BookLibrary &library)
{
    std::string isbn;
    std::cout << "Enter ISBN of the book to return: ";
    std::getline(std::cin, isbn);

    Book *book = library.lookupByIsbn(isbn);
    if (!book)
    {
        std::cout << "No book found with the ISBN " << isbn << std::endl;
        return;
    }
    if (!library.borrowBook(isbn))
    {
        std::cout << "The book: " << book->title << " isn't borrowed" << std::endl;
        return;
    }

    std::cout << "Return book: " << isbn << std::endl;
}

void printMenu()
{
    std::cout << std::endl
              << "Menu:" << std::endl;
    std::cout << "1. Add Book" << std::endl;
    std::cout << "2. Borrow Book" << std::endl;
    std::cout << "3. Return Book" << std::endl;
    std::cout << "4. Report" << std::endl;
    std::cout << "5. Exit" << std::endl;
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
            returnBook(library);
            break;
        case 4:
            library.statusReport();
            break;
        case 5:
            std::cout << "Exiting..." << std::endl;
            library.stopSweep();
            return 0;
        default:
            std::cout << "Invalid choice. Please try again" << std::endl;
        }
    }

    return 0;
}
