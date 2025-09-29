#ifndef POPULATE_H
#define POPULATE_H

#include "contact.h"

// Function declarations for dummy data population
void populateDummyContacts(AddressBook *book);
int addContactDirect(AddressBook *book, const char *name, const char *phone, 
                    const char *email, int roll_no, const char *department);

#endif // POPULATE_H