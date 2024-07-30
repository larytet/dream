# Dream Security Coding Exercise

In this exercise you will design and implement a `BookLibrary` class.  
`BookLibrary` is an in-memory database for `Book` objects.

## Book Properties

A `Book` has (at least) the following properties:

- Title
- Author Name
- ISBN
- Published Year

## BookLibrary Functionality

A `BookLibrary` should:

- Allow librarians to add a new book to the library.
- Allow librarians to remove a book from the library (permanently).
- Allow subscribers to lookup books in the library, by title or ISBN (exact match).
- Allow subscribers to borrow a book from the library.
- Allow subscribers to return a previously-borrowed book to the library.
- Maintain its own capacity by automatically scheduling an asynchronous book sweep whenever the book capacity is violated to remove the least-popular books.
- Allow librarians to get a status report of the number of books in the library, how many are borrowed, and how many are available.

## Assumptions

- An ISBN uniquely identifies a book.
- A title uniquely identifies a book.
- A book is popular if it was recently borrowed.
- The library can hold only one copy of the same book.
- A new book is rarely added to the library, while books are borrowed/returned frequently.

## Guidelines

- You may use any feature available in C++ 20.
- No other 3rd parties are allowed.
- Provide a simple console app that runs all the use-cases listed above.
- Printing is only allowed in the app or for debugging / error handling.
- Multiple subscribers / librarians may use the library from multiple threads.
- You may change the above class definitions to create additional/different abstractions, as you find suitable, as long as the use-cases are supported.
- Be mindful of memory management, ownership and synchronization.
- Document your code and any design or implementation decision you find worthy.


# Build

```bash
g++ console.cpp -o console
```


# Unitest 

```bash
cd test
cmake .
cmake --build .;gdb ./LRUCacheTest
```
