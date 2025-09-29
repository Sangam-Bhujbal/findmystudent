#ifndef CONTACT_H
#define CONTACT_H

#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_EMAIL_LEN 100
#define MAX_DEPT_LEN 50
#define INITIAL_CAPACITY 10

// Contact structure definition
typedef struct {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    int roll_no;
    char department[MAX_DEPT_LEN];
} Contact;

// AddressBook structure definition
typedef struct {
    Contact *contacts;
    int count;
    int capacity;
} AddressBook;

// Function declarations for contact management
void initializeAddressBook(AddressBook *book);
void freeAddressBook(AddressBook *book);
int addContact(AddressBook *book);
void listContacts(const AddressBook *book);
void searchContactMenu(const AddressBook *book);
int linearSearchByName(const AddressBook *book, const char *name);
int linearSearchByPhone(const AddressBook *book, const char *phone);
int linearSearchByRoll(const AddressBook *book, int roll_no);
int linearSearchByDepartment(const AddressBook *book, const char *department);
int binarySearchByName(const AddressBook *book, const char *name);
int binarySearchByRoll(const AddressBook *book, int roll_no);
void editContact(AddressBook *book);
void deleteContact(AddressBook *book);
void deleteAllContacts(AddressBook *book);
int validateName(const char *name);
int validatePhone(const char *phone);
int validateEmail(const char *email);
int validateRollNo(int roll_no, const AddressBook *book, int exclude_index);
void sortContactsByName(AddressBook *book);
void sortContactsByRoll(AddressBook *book);
void displayContact(const Contact *contact, int index);

#endif // CONTACT_H