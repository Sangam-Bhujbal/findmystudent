#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "populate.h"

// Add contact directly without user input (for populating dummy data)
int addContactDirect(AddressBook *book, const char *name, const char *phone, 
                    const char *email, int roll_no, const char *department) {
    if (book == NULL || name == NULL || phone == NULL || email == NULL || department == NULL) {
        return 0;
    }
    
    // Check if we need to resize the address book
    if (book->count >= book->capacity) {
        int new_capacity = book->capacity * 2;
        Contact *temp = realloc(book->contacts, new_capacity * sizeof(Contact));
        if (temp == NULL) {
            printf("Memory reallocation failed while adding dummy contact!\n");
            return 0;
        }
        book->contacts = temp;
        book->capacity = new_capacity;
    }
    
    // Validate the contact data
    if (!validateName(name) || !validatePhone(phone) || !validateEmail(email) || 
        !validateRollNo(roll_no, book, -1)) {
        printf("Warning: Invalid dummy data for contact %s, skipping.\n", name);
        return 0;
    }
    
    // Add the contact
    Contact *new_contact = &book->contacts[book->count];
    strcpy(new_contact->name, name);
    strcpy(new_contact->phone, phone);
    strcpy(new_contact->email, email);
    new_contact->roll_no = roll_no;
    strcpy(new_contact->department, department);
    
    book->count++;
    return 1;
}

// Populate address book with dummy contacts
void populateDummyContacts(AddressBook *book) {
    printf("\n=== Populating Address Book with Dummy Data ===\n");
    
    // Define dummy contacts data
    struct {
        const char *name;
        const char *phone;
        const char *email;
        int roll_no;
        const char *department;
    } dummy_contacts[] = {
        {"Alice Johnson", "1234567890", "alice.johnson@email.com", 101, "Computer Science"},
        {"Bob Smith", "2345678901", "bob.smith@email.com", 102, "Mechanical Engineering"},
        {"Carol Williams", "3456789012", "carol.williams@email.com", 103, "Electrical Engineering"},
        {"David Brown", "4567890123", "david.brown@email.com", 104, "Computer Science"},
        {"Emma Davis", "5678901234", "emma.davis@email.com", 105, "Civil Engineering"},
        {"Frank Wilson", "6789012345", "frank.wilson@email.com", 106, "Mechanical Engineering"},
        {"Grace Miller", "7890123456", "grace.miller@email.com", 107, "Electronics Engineering"},
        {"Henry Moore", "8901234567", "henry.moore@email.com", 108, "Computer Science"},
        {"Isabella Taylor", "9012345678", "isabella.taylor@email.com", 109, "Information Technology"},
        {"Jack Anderson", "0123456789", "jack.anderson@email.com", 110, "Electrical Engineering"},
        {"Kate Thomas", "1122334455", "kate.thomas@email.com", 111, "Computer Science"},
        {"Liam Jackson", "2233445566", "liam.jackson@email.com", 112, "Mechanical Engineering"},
        {"Mia White", "3344556677", "mia.white@email.com", 113, "Civil Engineering"},
        {"Noah Harris", "4455667788", "noah.harris@email.com", 114, "Electronics Engineering"},
        {"Olivia Martin", "5566778899", "olivia.martin@email.com", 115, "Information Technology"},
        {"Paul Thompson", "6677889900", "paul.thompson@email.com", 116, "Computer Science"},
        {"Quinn Garcia", "7788990011", "quinn.garcia@email.com", 117, "Electrical Engineering"},
        {"Rachel Rodriguez", "8899001122", "rachel.rodriguez@email.com", 118, "Mechanical Engineering"},
        {"Sam Lewis", "9900112233", "sam.lewis@email.com", 119, "Computer Science"},
        {"Tina Walker", "0011223344", "tina.walker@email.com", 120, "Information Technology"}
    };
    
    int total_contacts = sizeof(dummy_contacts) / sizeof(dummy_contacts[0]);
    int added_count = 0;
    
    // Add each dummy contact
    for (int i = 0; i < total_contacts; i++) {
        if (addContactDirect(book, 
                           dummy_contacts[i].name,
                           dummy_contacts[i].phone,
                           dummy_contacts[i].email,
                           dummy_contacts[i].roll_no,
                           dummy_contacts[i].department)) {
            added_count++;
        }
    }
    
    printf("Successfully added %d dummy contacts to the address book.\n", added_count);
    
    if (added_count > 0) {
        printf("\nDummy contacts include students from various departments:\n");
        printf("- Computer Science\n");
        printf("- Mechanical Engineering\n");
        printf("- Electrical Engineering\n");
        printf("- Civil Engineering\n");
        printf("- Electronics Engineering\n");
        printf("- Information Technology\n");
        printf("\nYou can now test the search, edit, and delete functions!\n");
    }
}

// Add sample contacts for specific testing scenarios
void populateTestContacts(AddressBook *book) {
    printf("\n=== Adding Test Contacts for Search Algorithm Testing ===\n");
    
    // Contacts designed to test search algorithms
    struct {
        const char *name;
        const char *phone;
        const char *email;
        int roll_no;
        const char *department;
    } test_contacts[] = {
        // Sorted names for binary search testing
        {"Aaron Adams", "1000000001", "aaron.adams@test.com", 1001, "Test Department"},
        {"Betty Brown", "1000000002", "betty.brown@test.com", 1002, "Test Department"},
        {"Charlie Clark", "1000000003", "charlie.clark@test.com", 1003, "Test Department"},
        {"Diana Davis", "1000000004", "diana.davis@test.com", 1004, "Test Department"},
        {"Edward Evans", "1000000005", "edward.evans@test.com", 1005, "Test Department"},
        // Duplicate department names for department search testing
        {"Frank Foster", "1000000006", "frank.foster@test.com", 1006, "Testing"},
        {"Grace Green", "1000000007", "grace.green@test.com", 1007, "Testing"},
        {"Harry Harris", "1000000008", "harry.harris@test.com", 1008, "Testing"}
    };
    
    int total_test_contacts = sizeof(test_contacts) / sizeof(test_contacts[0]);
    int added_count = 0;
    
    // Add each test contact
    for (int i = 0; i < total_test_contacts; i++) {
        if (addContactDirect(book, 
                           test_contacts[i].name,
                           test_contacts[i].phone,
                           test_contacts[i].email,
                           test_contacts[i].roll_no,
                           test_contacts[i].department)) {
            added_count++;
        }
    }
    
    printf("Successfully added %d test contacts for algorithm testing.\n", added_count);
    
    if (added_count > 0) {
        printf("\nTest contacts added:\n");
        printf("- Contacts with alphabetically sorted names (good for binary search)\n");
        printf("- Multiple contacts in 'Testing' department\n");
        printf("- Sequential roll numbers for binary search testing\n");
    }
}