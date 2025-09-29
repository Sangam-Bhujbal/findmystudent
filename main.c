#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

// Function declarations for menu functions
void displayMainMenu();
void displayAbout();
void displayHelp();
int getMenuChoice();
void pauseForUser();
void clearScreen();

// Main function
int main() {
    AddressBook addressBook;
    int choice;
    int running = 1;
    
    // Initialize the address book
    initializeAddressBook(&addressBook);
    
    // Load contacts from file at startup
    printf("=== Find My Student Application ===\n");
    printf("Loading contacts from %s...\n", CSV_FILENAME);
    loadContactsFromFile(&addressBook, CSV_FILENAME);
    
    // Main program loop
    while (running) {
        displayMainMenu();
        choice = getMenuChoice();
        
        // Check for EOF or special exit conditions
        if (choice == -2) { // Special return value for EOF
            printf("\nGracefully exiting...\n");
            break;
        }
        
        switch (choice) {
            case 1:
                addContact(&addressBook);
                pauseForUser();
                break;
                
            case 2:
                listContacts(&addressBook);
                pauseForUser();
                break;
                
            case 3:
                searchContactMenu(&addressBook);
                pauseForUser();
                break;
                
            case 4:
                editContact(&addressBook);
                pauseForUser();
                break;
                
            case 5:
                deleteContact(&addressBook);
                pauseForUser();
                break;
                
            case 6:
                deleteAllContacts(&addressBook);
                pauseForUser();
                break;
                
            case 7:
                printf("\n=== Save Contacts ===\n");
                if (saveContactsToFile(&addressBook, CSV_FILENAME)) {
                    printf("All contacts have been saved successfully!\n");
                } else {
                    printf("Failed to save contacts to file.\n");
                }
                pauseForUser();
                break;
                
            case 8:
                printf("\n=== Load Contacts ===\n");
                printf("Warning: This will replace all current contacts in memory.\n");
                printf("Make sure you have saved any unsaved changes!\n");
                printf("Do you want to continue? (y/N): ");
                char confirm;
                scanf("%c", &confirm);
                getchar(); // Consume newline
                
                if (confirm == 'y' || confirm == 'Y') {
                    // Clear current contacts
                    addressBook.count = 0;
                    // Load from file
                    if (loadContactsFromFile(&addressBook, CSV_FILENAME)) {
                        printf("Contacts loaded successfully!\n");
                    } else {
                        printf("Failed to load contacts from file.\n");
                    }
                } else {
                    printf("Load operation cancelled.\n");
                }
                pauseForUser();
                break;
                
            case 9:
                populateDummyContacts(&addressBook);
                pauseForUser();
                break;
                
            case 10:
                displayHelp();
                pauseForUser();
                break;
                
            case 11:
                displayAbout();
                pauseForUser();
                break;
                
            case 0:
                printf("\n=== Exit Application ===\n");
                printf("Do you want to save your contacts before exiting? (y/N): ");
                scanf("%c", &confirm);
                getchar(); // Consume newline
                
                if (confirm == 'y' || confirm == 'Y') {
                    if (saveContactsToFile(&addressBook, CSV_FILENAME)) {
                        printf("Contacts saved successfully!\n");
                    } else {
                        printf("Warning: Failed to save contacts!\n");
                    }
                }
                
                printf("Thank you for using Find My Student Application!\n");
                printf("Goodbye!\n");
                running = 0;
                break;
                
            default:
                printf("\nInvalid choice! Please enter a number between 0-11.\n");
                pauseForUser();
                break;
        }
        
        // Clear screen after each operation (optional)
        // clearScreen();
    }
    
    // Free allocated memory
    freeAddressBook(&addressBook);
    
    return 0;
}

// Display the main menu
void displayMainMenu() {
    printf("\n");
    printf("====================================================\n");
    printf("                FIND MY STUDENT                   \n");
    printf("====================================================\n");
    printf(" 1. Add New Contact                               \n");
    printf(" 2. List All Contacts                             \n");
    printf(" 3. Search Contacts                               \n");
    printf(" 4. Edit Contact                                  \n");
    printf(" 5. Delete Contact                                \n");
    printf(" 6. Delete All Contacts                           \n");
    printf(" 7. Save to File                                  \n");
    printf(" 8. Load from File                                \n");
    printf(" 9. Add Dummy Data                                \n");
    printf("10. Help                                          \n");
    printf("11. About                                         \n");
    printf(" 0. Exit                                          \n");
    printf("====================================================\n");
    printf("Enter your choice: ");
}

// Get menu choice from user
int getMenuChoice() {
    int choice;
    if (scanf("%d", &choice) != 1) {
        // Check if we hit EOF (pipe closed, Ctrl+D, etc.)
        if (feof(stdin)) {
            printf("\nInput stream closed. Exiting...\n");
            exit(0);
        }
        // Clear input buffer on invalid input
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            if (c == EOF) {
                printf("\nInput stream closed. Exiting...\n");
                exit(0);
            }
        }
        return -1; // Return invalid choice
    }
    
    // Consume newline
    int c = getchar();
    if (c == EOF) {
        printf("\nInput stream closed. Exiting...\n");
        exit(0);
    }
    
    return choice;
}

// Pause for user input
void pauseForUser() {
    printf("\nPress Enter to continue...");
    int c = getchar();
    if (c == EOF) {
        printf("\nInput stream closed. Exiting...\n");
        exit(0);
    }
}

// Clear screen (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Display help information
void displayHelp() {
    printf("\n=== FIND MY STUDENT HELP ===\n");
    printf("\nThis Find My Student application allows you to manage student contacts with the following features:\n\n");
    
    printf("MENU OPTIONS:\n");
    printf("1. Add New Contact - Add a new student contact with name, phone, email, roll number, and department\n");
    printf("2. List All Contacts - Display all contacts in a tabular format\n");
    printf("3. Search Contacts - Search for contacts using different criteria and algorithms:\n");
    printf("   • Search by Name (Linear or Binary Search)\n");
    printf("   • Search by Phone Number (Linear Search)\n");
    printf("   • Search by Roll Number (Linear or Binary Search)\n");
    printf("   • Search by Department (Linear Search - shows all matches)\n");
    printf("4. Edit Contact - Modify any field of an existing contact\n");
    printf("5. Delete Contact - Remove a contact from the address book\n");
    printf("6. Delete All Contacts - Remove ALL contacts from the address book (requires confirmation)\n");
    printf("7. Save to File - Save all contacts to contacts.csv file\n");
    printf("8. Load from File - Load contacts from contacts.csv file\n");
    printf("9. Add Dummy Data - Populate the address book with sample contacts for testing\n");
    printf("\nSEARCH ALGORITHMS:\n");
    printf("• Linear Search: Searches through all contacts sequentially (works on unsorted data)\n");
    printf("• Binary Search: Faster search that requires sorted data (automatically sorts when selected)\n");
    printf("\nINPUT VALIDATION:\n");
    printf("• Names: Only letters and spaces allowed (1-49 characters)\n");
    printf("• Phone: 10-14 characters with digits and optional +, -, (), spaces\n");
    printf("• Email: Must contain @ and . in correct positions\n");
    printf("• Roll Number: Must be positive and unique\n");
    printf("• Department: 1-49 characters allowed\n");
    printf("\nFILE OPERATIONS:\n");
    printf("• Data is stored in CSV format in contacts.csv\n");
    printf("• Automatic backup is created before saving\n");
    printf("• Application loads data automatically on startup\n");
}

// Display about information
void displayAbout() {
    printf("\n=== ABOUT FIND MY STUDENT ===\n");
    printf("\nFind My Student System\n");
    printf("Version: 1.0\n");
    printf("Developed by: SANGMESHWAR BHUJBAL\n");
    printf("\nDeveloped in C programming language\n");
    printf("\nFEATURES:\n");
    printf("• Complete CRUD operations (Create, Read, Update, Delete)\n");
    printf("• Multiple search algorithms (Linear and Binary Search)\n");
    printf("• Data persistence using CSV files\n");
    printf("• Input validation and error handling\n");
    printf("• Dynamic memory management\n");
    printf("• Automatic file backup before saves\n");
    printf("• Sample data population for testing\n");
    printf("\nTECH STACK:\n");
    printf("• Language: C\n");
    printf("• Data Structures: Structures, Arrays, Dynamic Memory\n");
    printf("• File I/O: Standard C file operations\n");
    printf("• String Processing: Standard C string functions\n");
    printf("\nUSE CASES:\n");
    printf("• Student contact management\n");
    printf("• Learning C programming concepts\n");
    printf("• Understanding data structures and algorithms\n");
    printf("• File handling and data persistence\n");
    printf("\nThis application demonstrates fundamental programming concepts\n");
    printf("including memory management, file I/O, searching algorithms,\n");
    printf("data validation, and user interface design.\n");
}