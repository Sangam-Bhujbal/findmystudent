#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file.h"

// Check if file exists
int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Create backup of existing file
void createBackup(const char *filename) {
    if (!fileExists(filename)) {
        return;
    }
    
    // Generate backup filename with timestamp
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    char backup_filename[256];
    snprintf(backup_filename, sizeof(backup_filename), 
             "%s.backup_%04d%02d%02d_%02d%02d%02d",
             filename,
             local_time->tm_year + 1900,
             local_time->tm_mon + 1,
             local_time->tm_mday,
             local_time->tm_hour,
             local_time->tm_min,
             local_time->tm_sec);
    
    FILE *source = fopen(filename, "r");
    FILE *backup = fopen(backup_filename, "w");
    
    if (source && backup) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), source)) {
            fputs(buffer, backup);
        }
        printf("Backup created: %s\n", backup_filename);
    }
    
    if (source) fclose(source);
    if (backup) fclose(backup);
}

// Save contacts to CSV file
int saveContactsToFile(const AddressBook *book, const char *filename) {
    if (book == NULL || filename == NULL) {
        printf("Error: Invalid parameters for saving contacts.\n");
        return 0;
    }
    
    // Create backup of existing file
    createBackup(filename);
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s for writing.\n", filename);
        return 0;
    }
    
    // Write CSV header
    fprintf(file, "Name,Phone,Email,Roll_No,Department\n");
    
    // Write contact data
    for (int i = 0; i < book->count; i++) {
        const Contact *contact = &book->contacts[i];
        
        // Handle commas in data by enclosing in quotes
        fprintf(file, "\"%s\",\"%s\",\"%s\",%d,\"%s\"\n",
                contact->name,
                contact->phone,
                contact->email,
                contact->roll_no,
                contact->department);
    }
    
    fclose(file);
    printf("Successfully saved %d contact(s) to %s\n", book->count, filename);
    return 1;
}

// Helper function to remove quotes from CSV field
static void removeQuotes(char *str) {
    int len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len-1] == '"') {
        // Remove first and last character (quotes)
        memmove(str, str + 1, len - 1);
        str[len - 2] = '\0';
    }
}

// Helper function to parse CSV line
static int parseCSVLine(char *line, Contact *contact) {
    char *token;
    int field = 0;
    
    // Remove trailing newline
    line[strcspn(line, "\n")] = 0;
    
    // Parse comma-separated values
    token = strtok(line, ",");
    while (token != NULL && field < 5) {
        switch (field) {
            case 0: // Name
                removeQuotes(token);
                strncpy(contact->name, token, MAX_NAME_LEN - 1);
                contact->name[MAX_NAME_LEN - 1] = '\0';
                break;
            case 1: // Phone
                removeQuotes(token);
                strncpy(contact->phone, token, MAX_PHONE_LEN - 1);
                contact->phone[MAX_PHONE_LEN - 1] = '\0';
                break;
            case 2: // Email
                removeQuotes(token);
                strncpy(contact->email, token, MAX_EMAIL_LEN - 1);
                contact->email[MAX_EMAIL_LEN - 1] = '\0';
                break;
            case 3: // Roll No
                contact->roll_no = atoi(token);
                break;
            case 4: // Department
                removeQuotes(token);
                strncpy(contact->department, token, MAX_DEPT_LEN - 1);
                contact->department[MAX_DEPT_LEN - 1] = '\0';
                break;
        }
        field++;
        token = strtok(NULL, ",");
    }
    
    return (field == 5); // Return 1 if all 5 fields were parsed successfully
}

// Resize address book for loading
static int ensureCapacity(AddressBook *book, int required_capacity) {
    if (required_capacity <= book->capacity) {
        return 1;
    }
    
    // Calculate new capacity (at least double current, but at least required)
    int new_capacity = book->capacity;
    while (new_capacity < required_capacity) {
        new_capacity *= 2;
    }
    
    Contact *temp = realloc(book->contacts, new_capacity * sizeof(Contact));
    if (temp == NULL) {
        printf("Error: Memory reallocation failed while loading contacts.\n");
        return 0;
    }
    
    book->contacts = temp;
    book->capacity = new_capacity;
    return 1;
}

// Load contacts from CSV file
int loadContactsFromFile(AddressBook *book, const char *filename) {
    if (book == NULL || filename == NULL) {
        printf("Error: Invalid parameters for loading contacts.\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Info: File %s not found. Starting with empty address book.\n", filename);
        return 1; // Not an error - file might not exist yet
    }
    
    char line[512];
    int line_number = 0;
    int loaded_count = 0;
    
    // Read file line by line
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        // Skip header line
        if (line_number == 1) {
            continue;
        }
        
        // Skip empty lines
        if (strlen(line) <= 1) {
            continue;
        }
        
        // Ensure we have enough capacity
        if (!ensureCapacity(book, book->count + 1)) {
            fclose(file);
            return 0;
        }
        
        // Parse the line
        Contact temp_contact;
        if (parseCSVLine(line, &temp_contact)) {
            // Validate the loaded contact
            if (validateName(temp_contact.name) &&
                validatePhone(temp_contact.phone) &&
                validateEmail(temp_contact.email) &&
                validateRollNo(temp_contact.roll_no, book, -1)) {
                
                // Add contact to address book
                book->contacts[book->count] = temp_contact;
                book->count++;
                loaded_count++;
            } else {
                printf("Warning: Invalid contact data on line %d, skipping.\n", line_number);
            }
        } else {
            printf("Warning: Could not parse line %d, skipping.\n", line_number);
        }
    }
    
    fclose(file);
    printf("Successfully loaded %d contact(s) from %s\n", loaded_count, filename);
    return 1;
}

// Advanced CSV parsing function (handles quoted fields with commas)
static int parseCSVLineAdvanced(char *line, Contact *contact) {
    char *fields[5] = {NULL};
    int field_count = 0;
    char *ptr = line;
    int in_quotes = 0;
    char *field_start = ptr;
    
    // Remove trailing newline
    line[strcspn(line, "\n")] = 0;
    
    while (*ptr && field_count < 5) {
        if (*ptr == '"' && (ptr == field_start || *(ptr-1) == ',')) {
            in_quotes = !in_quotes;
            // Remove the quote by shifting the string
            memmove(ptr, ptr + 1, strlen(ptr));
            continue;
        } else if (*ptr == '"' && in_quotes) {
            in_quotes = !in_quotes;
            // Remove the quote by shifting the string
            memmove(ptr, ptr + 1, strlen(ptr));
            continue;
        } else if (*ptr == ',' && !in_quotes) {
            *ptr = '\0';
            fields[field_count] = field_start;
            field_count++;
            field_start = ptr + 1;
        }
        ptr++;
    }
    
    // Add the last field
    if (field_count < 5) {
        fields[field_count] = field_start;
        field_count++;
    }
    
    // Parse fields into contact structure
    if (field_count == 5) {
        strncpy(contact->name, fields[0], MAX_NAME_LEN - 1);
        contact->name[MAX_NAME_LEN - 1] = '\0';
        
        strncpy(contact->phone, fields[1], MAX_PHONE_LEN - 1);
        contact->phone[MAX_PHONE_LEN - 1] = '\0';
        
        strncpy(contact->email, fields[2], MAX_EMAIL_LEN - 1);
        contact->email[MAX_EMAIL_LEN - 1] = '\0';
        
        contact->roll_no = atoi(fields[3]);
        
        strncpy(contact->department, fields[4], MAX_DEPT_LEN - 1);
        contact->department[MAX_DEPT_LEN - 1] = '\0';
        
        return 1;
    }
    
    return 0;
}