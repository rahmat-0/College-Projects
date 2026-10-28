/* UID - 26BCD10036
   Name - Rahmat Ansari
   Project - Library Management */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "library.dat"
#define TEMP_FILE "temp.dat"

// book record
struct Book {
    int  id;
    char title[60];
    char author[40];
    int  available;      // 1 = available, 0 = issued
};

// input helpers

int readInt(const char *prompt) {
    char buf[50];
    int n;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            exit(0);
        }
        if (sscanf(buf, "%d", &n) == 1) {
            return n;
        }
        printf("Invalid number! Please try again.\n");
    }
}

void readText(const char *prompt, char dest[], int size) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) {
        exit(0);
    }
    dest[strcspn(dest, "\n")] = '\0';    // strip newline
}

void toLower(char s[]) {
    for (int i = 0; s[i] != '\0'; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

// main functions

int idExists(int id) {
    struct Book b;
    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        return 0;
    }
    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void addBook(void) {
    struct Book b;
    FILE *fp;

    b.id = readInt("Enter Book ID      : ");
    if (idExists(b.id)) {
        printf("A book with this ID already exists!\n");
        return;
    }
    readText("Enter Title        : ", b.title, sizeof(b.title));
    readText("Enter Author       : ", b.author, sizeof(b.author));
    b.available = 1;

    fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        printf("Error: could not open file.\n");
        return;
    }
    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void displayBooks(void) {
    struct Book b;
    int count = 0;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No books in the library yet.\n");
        return;
    }
    printf("\n%-6s %-30s %-20s %s\n", "ID", "Title", "Author", "Status");
    printf("--------------------------------------------------------------------\n");
    while (fread(&b, sizeof(b), 1, fp) == 1) {
        printf("%-6d %-30s %-20s %s\n", b.id, b.title, b.author,
               b.available ? "Available" : "Issued");
        count++;
    }
    fclose(fp);
    if (count == 0) {
        printf("No books in the library yet.\n");
    } else {
        printf("--------------------------------------------------------------------\n");
        printf("Total books: %d\n", count);
    }
}

// search by title or author
void searchBook(void) {
    struct Book b;
    char key[60], title[60], author[40];
    int found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No books in the library yet.\n");
        return;
    }
    readText("Enter title or author to search: ", key, sizeof(key));
    toLower(key);

    printf("\n%-6s %-30s %-20s %s\n", "ID", "Title", "Author", "Status");
    printf("--------------------------------------------------------------------\n");
    while (fread(&b, sizeof(b), 1, fp) == 1) {
        strcpy(title, b.title);
        strcpy(author, b.author);
        toLower(title);
        toLower(author);
        if (strstr(title, key) != NULL || strstr(author, key) != NULL) {
            printf("%-6d %-30s %-20s %s\n", b.id, b.title, b.author,
                   b.available ? "Available" : "Issued");
            found = 1;
        }
    }
    fclose(fp);
    if (!found) {
        printf("No matching book found.\n");
    }
}

// newStatus 0 = issue, 1 = return
void changeStatus(int newStatus) {
    struct Book b;
    int id, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb+");

    if (fp == NULL) {
        printf("No books in the library yet.\n");
        return;
    }
    id = readInt("Enter Book ID: ");

    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
            if (b.available == newStatus) {
                if (newStatus == 0) {
                    printf("This book is already issued.\n");
                } else {
                    printf("This book is already in the library.\n");
                }
            } else {
                b.available = newStatus;
                fseek(fp, -(long)sizeof(b), SEEK_CUR);   // back one record
                fwrite(&b, sizeof(b), 1, fp);
                if (newStatus == 0) {
                    printf("Book \"%s\" issued successfully.\n", b.title);
                } else {
                    printf("Book \"%s\" returned successfully.\n", b.title);
                }
            }
            break;
        }
    }
    fclose(fp);
    if (!found) {
        printf("Book ID not found.\n");
    }
}

void deleteBook(void) {
    struct Book b;
    int id, found = 0;
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp;

    if (fp == NULL) {
        printf("No books in the library yet.\n");
        return;
    }
    temp = fopen(TEMP_FILE, "wb");
    if (temp == NULL) {
        printf("Error: could not create temporary file.\n");
        fclose(fp);
        return;
    }
    id = readInt("Enter Book ID to delete: ");

    // copy all books except the deleted one
    while (fread(&b, sizeof(b), 1, fp) == 1) {
        if (b.id == id) {
            found = 1;
        } else {
            fwrite(&b, sizeof(b), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename(TEMP_FILE, FILE_NAME);

    if (found) {
        printf("Book deleted successfully.\n");
    } else {
        printf("Book ID not found.\n");
    }
}

// menu

int main(void) {
    int choice;
    do {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Exit\n");
        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: addBook();         break;
            case 2: displayBooks();    break;
            case 3: searchBook();      break;
            case 4: changeStatus(0);   break;
            case 5: changeStatus(1);   break;
            case 6: deleteBook();      break;
            case 7: printf("Thank you! Goodbye.\n"); break;
            default: printf("Invalid choice, try again.\n");
        }
    } while (choice != 7);

    return 0;
}