# Library Management System

A simple command-line library management program written in C. It stores book records in a binary file, so your data stays saved after you close the program.

## Features

- Add a new book
- Display all books
- Search books by title or author
- Issue a book
- Return a book
- Delete a book

## How to Run

Compile the program with GCC:

```
gcc library.c -o library
```

Run it:

```
./library
```

On Windows, run `library.exe` instead.

## Usage

When the program starts, you will see a menu:

```
===== LIBRARY MANAGEMENT SYSTEM =====
1. Add Book
2. Display All Books
3. Search Book
4. Issue Book
5. Return Book
6. Delete Book
7. Exit
```

Type the number of the option you want and press Enter.

## Notes

- Each book needs a unique ID.
- Book data is saved in `library.dat`, which is created automatically.
- Search is not case sensitive.

## Author

Rahmat Ansari
