#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"

// Initialize the address book
void initializeAddressBook(AddressBook *book) {
    book->contacts = malloc(INITIAL_CAPACITY * sizeof(Contact));
    if (book->contacts == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    book->count = 0;
    book->capacity = INITIAL_CAPACITY;
}

// Free memory allocated for address book
void freeAddressBook(AddressBook *book) {
    if (book->contacts) {
        free(book->contacts);
        book->contacts = NULL;
    }
    book->count = 0;
    book->capacity = 0;
}

// Resize address book if needed
static int resizeAddressBook(AddressBook *book) {
    if (book->count >= book->capacity) {
        int new_capacity = book->capacity * 2;
        Contact *temp = realloc(book->contacts, new_capacity * sizeof(Contact));
        if (temp == NULL) {
            printf("Memory reallocation failed!\n");
            return 0;
        }
        book->contacts = temp;
        book->capacity = new_capacity;
    }
    return 1;
}

// Validate name input
int validateName(const char *name) {
    if (strlen(name) == 0 || strlen(name) >= MAX_NAME_LEN) {
        return 0;
    }
    // Check if name contains only letters and spaces
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Validate phone number
int validatePhone(const char *phone) {
    if (strlen(phone) < 10 || strlen(phone) >= MAX_PHONE_LEN) {
        return 0;
    }
    // Check if phone contains only digits, +, -, (, ), and spaces
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-' && 
            phone[i] != '(' && phone[i] != ')' && phone[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

// Validate email
int validateEmail(const char *email) {
    if (strlen(email) == 0 || strlen(email) >= MAX_EMAIL_LEN) {
        return 0;
    }
    // Simple email validation - must contain @ and .
    char *at_pos = strchr(email, '@');
    char *dot_pos = strrchr(email, '.');
    if (at_pos == NULL || dot_pos == NULL || at_pos > dot_pos) {
        return 0;
    }
    return 1;
}

// Validate roll number (must be unique)
int validateRollNo(int roll_no, const AddressBook *book, int exclude_index) {
    if (roll_no <= 0) {
        return 0;
    }
    for (int i = 0; i < book->count; i++) {
        if (i != exclude_index && book->contacts[i].roll_no == roll_no) {
            return 0; // Roll number already exists
        }
    }
    return 1;
}

// Add a new contact
int addContact(AddressBook *book) {
    if (!resizeAddressBook(book)) {
        return 0;
    }

    Contact new_contact;
    char buffer[256];
    
    printf("\n=== Add New Contact ===\n");
    
    // Get name
    do {
        printf("Enter name: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
        if (!validateName(buffer)) {
            printf("Invalid name! Name should contain only letters and spaces (1-49 characters).\n");
        }
    } while (!validateName(buffer));
    strcpy(new_contact.name, buffer);
    
    // Get phone
    do {
        printf("Enter phone number: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (!validatePhone(buffer)) {
            printf("Invalid phone number! Must be 10-14 characters with digits and optional +, -, (), spaces.\n");
        }
    } while (!validatePhone(buffer));
    strcpy(new_contact.phone, buffer);
    
    // Get email
    do {
        printf("Enter email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (!validateEmail(buffer)) {
            printf("Invalid email! Must contain @ and . in correct positions.\n");
        }
    } while (!validateEmail(buffer));
    strcpy(new_contact.email, buffer);
    
    // Get roll number
    do {
        printf("Enter roll number: ");
        scanf("%d", &new_contact.roll_no);
        getchar(); // Consume newline
        if (!validateRollNo(new_contact.roll_no, book, -1)) {
            printf("Invalid roll number! Must be positive and unique.\n");
        }
    } while (!validateRollNo(new_contact.roll_no, book, -1));
    
    // Get department
    do {
        printf("Enter department: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0 || strlen(buffer) >= MAX_DEPT_LEN) {
            printf("Invalid department! Must be 1-49 characters.\n");
        }
    } while (strlen(buffer) == 0 || strlen(buffer) >= MAX_DEPT_LEN);
    strcpy(new_contact.department, buffer);
    
    // Add contact to address book
    book->contacts[book->count] = new_contact;
    book->count++;
    
    printf("Contact added successfully!\n");
    return 1;
}

// Display a single contact
void displayContact(const Contact *contact, int index) {
    printf("%-4d %-20s %-15s %-30s %-8d %-15s\n", 
           index + 1, contact->name, contact->phone, 
           contact->email, contact->roll_no, contact->department);
}

// List all contacts
void listContacts(const AddressBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts found in Find My Student.\n");
        return;
    }
    
    printf("\n=== Find My Student Contacts ===\n");
    printf("%-4s %-20s %-15s %-30s %-8s %-15s\n", 
           "No.", "Name", "Phone", "Email", "Roll No", "Department");
    printf("================================================================================\n");
    
    for (int i = 0; i < book->count; i++) {
        displayContact(&book->contacts[i], i);
    }
    printf("\nTotal contacts: %d\n", book->count);
}

// Linear search by name
int linearSearchByName(const AddressBook *book, const char *name) {
    for (int i = 0; i < book->count; i++) {
        if (strcasecmp(book->contacts[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Linear search by phone
int linearSearchByPhone(const AddressBook *book, const char *phone) {
    for (int i = 0; i < book->count; i++) {
        if (strcmp(book->contacts[i].phone, phone) == 0) {
            return i;
        }
    }
    return -1;
}

// Linear search by roll number
int linearSearchByRoll(const AddressBook *book, int roll_no) {
    for (int i = 0; i < book->count; i++) {
        if (book->contacts[i].roll_no == roll_no) {
            return i;
        }
    }
    return -1;
}

// Linear search by department
int linearSearchByDepartment(const AddressBook *book, const char *department) {
    printf("\n=== Contacts in %s Department ===\n", department);
    printf("%-4s %-20s %-15s %-30s %-8s %-15s\n", 
           "No.", "Name", "Phone", "Email", "Roll No", "Department");
    printf("================================================================================\n");
    
    int found = 0;
    for (int i = 0; i < book->count; i++) {
        if (strcasecmp(book->contacts[i].department, department) == 0) {
            displayContact(&book->contacts[i], i);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No contacts found in %s department.\n", department);
        return -1;
    }
    printf("\nFound %d contact(s) in %s department.\n", found, department);
    return found;
}

// Sort contacts by name for binary search
void sortContactsByName(AddressBook *book) {
    for (int i = 0; i < book->count - 1; i++) {
        for (int j = 0; j < book->count - i - 1; j++) {
            if (strcasecmp(book->contacts[j].name, book->contacts[j + 1].name) > 0) {
                Contact temp = book->contacts[j];
                book->contacts[j] = book->contacts[j + 1];
                book->contacts[j + 1] = temp;
            }
        }
    }
}

// Sort contacts by roll number for binary search
void sortContactsByRoll(AddressBook *book) {
    for (int i = 0; i < book->count - 1; i++) {
        for (int j = 0; j < book->count - i - 1; j++) {
            if (book->contacts[j].roll_no > book->contacts[j + 1].roll_no) {
                Contact temp = book->contacts[j];
                book->contacts[j] = book->contacts[j + 1];
                book->contacts[j + 1] = temp;
            }
        }
    }
}

// Binary search by name
int binarySearchByName(const AddressBook *book, const char *name) {
    int left = 0, right = book->count - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcasecmp(book->contacts[mid].name, name);
        
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Binary search by roll number
int binarySearchByRoll(const AddressBook *book, int roll_no) {
    int left = 0, right = book->count - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (book->contacts[mid].roll_no == roll_no) {
            return mid;
        } else if (book->contacts[mid].roll_no < roll_no) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Search contact menu
void searchContactMenu(const AddressBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts available to search.\n");
        return;
    }
    
    int choice, search_type;
    char search_term[256];
    int roll_no, result;
    
    printf("\n=== Search Contacts ===\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone\n");
    printf("3. Search by Roll Number\n");
    printf("4. Search by Department\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline
    
    switch (choice) {
        case 1:
            printf("Enter name to search: ");
            fgets(search_term, sizeof(search_term), stdin);
            search_term[strcspn(search_term, "\n")] = 0;
            
            printf("\nChoose search algorithm:\n");
            printf("1. Linear Search\n");
            printf("2. Binary Search (will sort contacts first)\n");
            printf("Enter choice: ");
            scanf("%d", &search_type);
            getchar();
            
            if (search_type == 2) {
                printf("Sorting contacts by name for binary search...\n");
                sortContactsByName((AddressBook *)book); // Cast away const for sorting
                result = binarySearchByName(book, search_term);
            } else {
                result = linearSearchByName(book, search_term);
            }
            
            if (result != -1) {
                printf("\n=== Contact Found ===\n");
                printf("%-4s %-20s %-15s %-30s %-8s %-15s\n", 
                       "No.", "Name", "Phone", "Email", "Roll No", "Department");
                printf("================================================================================\n");
                displayContact(&book->contacts[result], result);
            } else {
                printf("Contact with name '%s' not found.\n", search_term);
            }
            break;
            
        case 2:
            printf("Enter phone number to search: ");
            fgets(search_term, sizeof(search_term), stdin);
            search_term[strcspn(search_term, "\n")] = 0;
            
            result = linearSearchByPhone(book, search_term);
            if (result != -1) {
                printf("\n=== Contact Found ===\n");
                printf("%-4s %-20s %-15s %-30s %-8s %-15s\n", 
                       "No.", "Name", "Phone", "Email", "Roll No", "Department");
                printf("================================================================================\n");
                displayContact(&book->contacts[result], result);
            } else {
                printf("Contact with phone '%s' not found.\n", search_term);
            }
            break;
            
        case 3:
            printf("Enter roll number to search: ");
            scanf("%d", &roll_no);
            getchar();
            
            printf("\nChoose search algorithm:\n");
            printf("1. Linear Search\n");
            printf("2. Binary Search (will sort contacts first)\n");
            printf("Enter choice: ");
            scanf("%d", &search_type);
            getchar();
            
            if (search_type == 2) {
                printf("Sorting contacts by roll number for binary search...\n");
                sortContactsByRoll((AddressBook *)book); // Cast away const for sorting
                result = binarySearchByRoll(book, roll_no);
            } else {
                result = linearSearchByRoll(book, roll_no);
            }
            
            if (result != -1) {
                printf("\n=== Contact Found ===\n");
                printf("%-4s %-20s %-15s %-30s %-8s %-15s\n", 
                       "No.", "Name", "Phone", "Email", "Roll No", "Department");
                printf("================================================================================\n");
                displayContact(&book->contacts[result], result);
            } else {
                printf("Contact with roll number %d not found.\n", roll_no);
            }
            break;
            
        case 4:
            printf("Enter department to search: ");
            fgets(search_term, sizeof(search_term), stdin);
            search_term[strcspn(search_term, "\n")] = 0;
            
            linearSearchByDepartment(book, search_term);
            break;
            
        default:
            printf("Invalid choice!\n");
    }
}

// Edit contact
void editContact(AddressBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts available to edit.\n");
        return;
    }
    
    listContacts(book);
    
    int index;
    printf("\nEnter contact number to edit (1-%d): ", book->count);
    scanf("%d", &index);
    getchar(); // Consume newline
    
    if (index < 1 || index > book->count) {
        printf("Invalid contact number!\n");
        return;
    }
    
    index--; // Convert to 0-based index
    Contact *contact = &book->contacts[index];
    
    printf("\n=== Edit Contact ===\n");
    printf("Current details:\n");
    displayContact(contact, index);
    
    char buffer[256];
    int choice;
    
    do {
        printf("\nWhat would you like to edit?\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("4. Roll Number\n");
        printf("5. Department\n");
        printf("6. Done editing\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch (choice) {
            case 1:
                do {
                    printf("Enter new name: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (!validateName(buffer)) {
                        printf("Invalid name! Name should contain only letters and spaces (1-49 characters).\n");
                    }
                } while (!validateName(buffer));
                strcpy(contact->name, buffer);
                printf("Name updated successfully!\n");
                break;
                
            case 2:
                do {
                    printf("Enter new phone: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (!validatePhone(buffer)) {
                        printf("Invalid phone number! Must be 10-14 characters with digits and optional +, -, (), spaces.\n");
                    }
                } while (!validatePhone(buffer));
                strcpy(contact->phone, buffer);
                printf("Phone updated successfully!\n");
                break;
                
            case 3:
                do {
                    printf("Enter new email: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (!validateEmail(buffer)) {
                        printf("Invalid email! Must contain @ and . in correct positions.\n");
                    }
                } while (!validateEmail(buffer));
                strcpy(contact->email, buffer);
                printf("Email updated successfully!\n");
                break;
                
            case 4:
                do {
                    printf("Enter new roll number: ");
                    scanf("%d", &contact->roll_no);
                    getchar();
                    if (!validateRollNo(contact->roll_no, book, index)) {
                        printf("Invalid roll number! Must be positive and unique.\n");
                    }
                } while (!validateRollNo(contact->roll_no, book, index));
                printf("Roll number updated successfully!\n");
                break;
                
            case 5:
                do {
                    printf("Enter new department: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) == 0 || strlen(buffer) >= MAX_DEPT_LEN) {
                        printf("Invalid department! Must be 1-49 characters.\n");
                    }
                } while (strlen(buffer) == 0 || strlen(buffer) >= MAX_DEPT_LEN);
                strcpy(contact->department, buffer);
                printf("Department updated successfully!\n");
                break;
                
            case 6:
                printf("Editing completed.\n");
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 6);
}

// Delete contact
void deleteContact(AddressBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts available to delete.\n");
        return;
    }
    
    listContacts(book);
    
    int index;
    printf("\nEnter contact number to delete (1-%d): ", book->count);
    scanf("%d", &index);
    getchar(); // Consume newline
    
    if (index < 1 || index > book->count) {
        printf("Invalid contact number!\n");
        return;
    }
    
    index--; // Convert to 0-based index
    
    printf("\nContact to be deleted:\n");
    printf("%-4s %-20s %-15s %-30s %-8s %-15s\n", 
           "No.", "Name", "Phone", "Email", "Roll No", "Department");
    printf("================================================================================\n");
    displayContact(&book->contacts[index], index);
    
    char confirm;
    printf("\nAre you sure you want to delete this contact? (y/N): ");
    scanf("%c", &confirm);
    getchar(); // Consume newline
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift all contacts after the deleted one
        for (int i = index; i < book->count - 1; i++) {
            book->contacts[i] = book->contacts[i + 1];
        }
        book->count--;
        printf("Contact deleted successfully!\n");
    } else {
        printf("Contact deletion cancelled.\n");
    }
}

// Delete all contacts
void deleteAllContacts(AddressBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts available to delete.\n");
        return;
    }
    
    printf("\n=== Delete All Contacts ===\n");
    printf("Warning: This operation will permanently delete ALL %d contact(s) from Find My Student.\n", book->count);
    printf("This action cannot be undone!\n\n");
    
    printf("Current contacts in Find My Student:\n");
    listContacts(book);
    
    char confirm[20];
    printf("\nAre you absolutely sure you want to delete ALL contacts? \n");
    printf("Type 'DELETE ALL' to confirm (case sensitive): ");
    
    // Read the entire line to handle multi-word confirmation
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = 0; // Remove newline
    
    if (strcmp(confirm, "DELETE ALL") == 0) {
        // Clear all contacts
        book->count = 0;
        printf("\nAll contacts have been deleted successfully!\n");
        printf("Find My Student is now empty.\n");
    } else {
        printf("\nDelete all operation cancelled.\n");
        printf("No contacts were deleted.\n");
    }
}
