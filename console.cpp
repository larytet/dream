#include <iostream>
#include <thread>
#include "library.hpp"

void librarianActions(BookLibrary& library) {
    library.addBook(Book("The Catcher in the Rye", "J.D. Salinger", "1234567890", 1951));
    library.addBook(Book("To Kill a Mockingbird", "Harper Lee", "2345678901", 1960));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    library.removeBook("2345678901");
}

void subscriberActions(BookLibrary& library) {
    Book* book = library.lookupByIsbn("1234567890");
    if (book) {
        std::cout << "Found book: " << book->title << " by " << book->author << std::endl;
        if (library.borrowBook("1234567890")) {
            std::cout << "Borrowed book: " << book->title << std::endl;
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    library.returnBook("1234567890");
}

int main() {
    BookLibrary library(10);

    std::thread librarian(librarianActions, std::ref(library));
    std::thread subscriber(subscriberActions, std::ref(library));

    librarian.join();
    subscriber.join();

    library.statusReport();

    return 0;
}
