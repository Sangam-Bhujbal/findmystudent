#ifndef FILE_H
#define FILE_H

#include "contact.h"

#define CSV_FILENAME "contacts.csv"

// Function declarations for file operations
int saveContactsToFile(const AddressBook *book, const char *filename);
int loadContactsFromFile(AddressBook *book, const char *filename);
void createBackup(const char *filename);
int fileExists(const char *filename);

#endif // FILE_H