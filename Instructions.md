# Address Book - Detailed Instructions

This document provides comprehensive instructions for using the Address Book Management System.

## Table of Contents
1. [Getting Started](#getting-started)
2. [Menu Navigation](#menu-navigation)
3. [Adding Contacts](#adding-contacts)
4. [Viewing Contacts](#viewing-contacts)
5. [Searching Contacts](#searching-contacts)
6. [Editing Contacts](#editing-contacts)
7. [Deleting Contacts](#deleting-contacts)
8. [File Operations](#file-operations)
9. [Search Algorithms Explained](#search-algorithms-explained)
10. [Input Validation Rules](#input-validation-rules)
11. [Troubleshooting](#troubleshooting)

## Getting Started

### Compilation
Before using the application, compile it using one of these methods:

**For GCC (recommended):**
```bash
gcc -o addressbook main.c contact.c file.c populate.c -std=c99
```

**For Windows with MinGW:**
```cmd
gcc -o addressbook.exe main.c contact.c file.c populate.c -std=c99
```

**For Visual Studio:**
```cmd
cl main.c contact.c file.c populate.c /Fe:addressbook.exe
```

### Running the Application
```bash
./addressbook        # Linux/macOS
addressbook.exe      # Windows
```

The application will automatically:
- Load existing contacts from `contacts.csv` (if it exists)
- Display the main menu
- Wait for your input

## Menu Navigation

The main menu provides 11 options (0-10):

```
====================================================
         ADDRESS BOOK MANAGEMENT SYSTEM           
====================================================
 1. Add New Contact                               
 2. List All Contacts                             
 3. Search Contacts                               
 4. Edit Contact                                  
 5. Delete Contact                                
 6. Save to File                                  
 7. Load from File                                
 8. Add Dummy Data                                
 9. Help                                          
10. About                                         
 0. Exit                                          
====================================================
```

- Enter the number corresponding to your choice
- Press Enter to confirm
- Invalid inputs will show an error message

## Adding Contacts

**Menu Option: 1**

When adding a new contact, you'll be prompted for:

1. **Name**: 
   - Enter the contact's full name
   - Only letters and spaces allowed
   - Must be 1-49 characters

2. **Phone Number**:
   - Enter phone number (10-14 characters)
   - Can include: digits, +, -, (), spaces
   - Example: `+1 (555) 123-4567` or `5551234567`

3. **Email**:
   - Enter valid email address
   - Must contain @ and . in correct positions
   - Example: `john.doe@example.com`

4. **Roll Number**:
   - Enter positive integer
   - Must be unique (no duplicates allowed)
   - Example: `101`, `2023001`

5. **Department**:
   - Enter department name (1-49 characters)
   - Example: `Computer Science`, `Mechanical Engineering`

### Example Add Contact Session:
```
=== Add New Contact ===
Enter name: John Smith
Enter phone number: +1-555-1234567
Enter email: john.smith@university.edu
Enter roll number: 12345
Enter department: Computer Science
Contact added successfully!
```

## Viewing Contacts

**Menu Option: 2**

Displays all contacts in a formatted table:

```
=== Address Book Contacts ===
No.  Name                 Phone           Email                          Roll No  Department     
================================================================================
1    John Smith           +1-555-1234567  john.smith@university.edu      12345    Computer Science
2    Jane Doe             555-987-6543    jane.doe@university.edu        12346    Electrical Engineering

Total contacts: 2
```

- Shows all contact information in columns
- Displays total number of contacts
- If no contacts exist, shows "No contacts found" message

## Searching Contacts

**Menu Option: 3**

The search menu offers multiple search options:

```
=== Search Contacts ===
1. Search by Name
2. Search by Phone
3. Search by Roll Number
4. Search by Department
Enter your choice: 
```

### 1. Search by Name
- Choose between Linear Search or Binary Search
- Binary search will automatically sort contacts first
- Case-insensitive search
- Must match exact name

**Example:**
```
Enter name to search: John Smith
Choose search algorithm:
1. Linear Search
2. Binary Search (will sort contacts first)
Enter choice: 1
```

### 2. Search by Phone
- Uses Linear Search only
- Must match exact phone number
- Case-sensitive for special characters

### 3. Search by Roll Number
- Choose between Linear Search or Binary Search
- Binary search will automatically sort contacts first
- Must match exact roll number

### 4. Search by Department
- Uses Linear Search
- Shows ALL contacts in the specified department
- Case-insensitive search

**Example Department Search:**
```
Enter department to search: Computer Science

=== Contacts in Computer Science Department ===
No.  Name                 Phone           Email                          Roll No  Department     
================================================================================
1    John Smith           555-1234567     john.smith@uni.edu            12345    Computer Science
3    Alice Johnson        555-9876543     alice.johnson@uni.edu         12347    Computer Science

Found 2 contact(s) in Computer Science department.
```

## Editing Contacts

**Menu Option: 4**

1. All contacts are displayed with numbers
2. Enter the number of the contact to edit
3. Choose which field to modify:
   - Name
   - Phone
   - Email
   - Roll Number
   - Department
4. Enter new value (with validation)
5. Repeat for multiple fields or choose "Done editing"

**Example Edit Session:**
```
Enter contact number to edit (1-5): 2

=== Edit Contact ===
Current details:
2    Jane Doe             555-987-6543    jane.doe@uni.edu              12346    Electrical Engineering

What would you like to edit?
1. Name
2. Phone
3. Email
4. Roll Number
5. Department
6. Done editing
Enter your choice: 3
Enter new email: jane.doe@newuniversity.edu
Email updated successfully!
```

## Deleting Contacts

**Menu Option: 5**

1. All contacts are displayed with numbers
2. Enter the number of the contact to delete
3. Contact details are shown for confirmation
4. Confirm deletion (y/N)
5. Contact is permanently removed

**Example Delete Session:**
```
Enter contact number to delete (1-3): 2

Contact to be deleted:
No.  Name                 Phone           Email                          Roll No  Department     
================================================================================
2    Jane Doe             555-987-6543    jane.doe@uni.edu              12346    Electrical Engineering

Are you sure you want to delete this contact? (y/N): y
Contact deleted successfully!
```

## File Operations

### Save to File (Option 6)
- Saves all current contacts to `contacts.csv`
- Creates automatic backup of existing file
- Backup format: `contacts.csv.backup_YYYYMMDD_HHMMSS`
- Shows confirmation message

### Load from File (Option 7)
- **Warning**: Replaces all contacts currently in memory
- Loads contacts from `contacts.csv`
- Shows warning and requires confirmation
- Validates loaded data

### Add Dummy Data (Option 8)
- Adds 20 sample contacts for testing
- Includes various departments
- Useful for testing search algorithms
- Won't add duplicate roll numbers

## Search Algorithms Explained

### Linear Search
- **How it works**: Checks each contact one by one from start to finish
- **Time**: O(n) - Gets slower with more contacts
- **Advantage**: Works with unsorted data
- **Best for**: Small datasets or frequently changing data
- **Available for**: All search types

### Binary Search
- **How it works**: Divides search space in half each time
- **Time**: O(log n) - Much faster for large datasets
- **Requirement**: Data must be sorted first
- **Process**: Automatically sorts contacts before searching
- **Best for**: Large datasets that don't change often
- **Available for**: Name and Roll Number searches only

### When to Use Each:
- **Linear Search**: 
  - Few contacts (< 100)
  - Data changes frequently
  - Searching by phone or department
- **Binary Search**:
  - Many contacts (> 100)
  - Data is relatively stable
  - Maximum search speed needed

## Input Validation Rules

### Name Validation
- **Length**: 1-49 characters
- **Characters**: Letters and spaces only
- **Invalid**: Numbers, special characters, empty
- **Examples**: 
  - ✅ "John Smith", "Mary Jane Watson"
  - ❌ "John123", "John@Smith", "", "J"

### Phone Validation
- **Length**: 10-14 characters
- **Characters**: Digits, +, -, (, ), spaces
- **Invalid**: Letters, other special characters
- **Examples**:
  - ✅ "1234567890", "+1-555-123-4567", "(555) 123-4567"
  - ❌ "123-45-6789" (too short), "phone123", "555.123.4567"

### Email Validation
- **Requirements**: Must contain @ and . in correct order
- **Length**: 1-99 characters
- **Format**: text@domain.extension
- **Examples**:
  - ✅ "user@example.com", "student.name@university.edu"
  - ❌ "user@com", "user.example.com", "@example.com"

### Roll Number Validation
- **Type**: Positive integer only
- **Uniqueness**: Must not already exist in address book
- **Range**: 1 to 2,147,483,647
- **Examples**:
  - ✅ 1, 12345, 2023001
  - ❌ 0, -5, 12345 (if already exists)

### Department Validation
- **Length**: 1-49 characters
- **Characters**: Any printable characters allowed
- **Invalid**: Empty string only
- **Examples**:
  - ✅ "Computer Science", "Mech. Engineering", "IT & CS"
  - ❌ "" (empty)

## Troubleshooting

### Common Issues

**1. "File contacts.csv not found" on startup**
- This is normal for first run
- Application starts with empty address book
- Add contacts or use "Add Dummy Data" option

**2. "Memory allocation failed"**
- System is out of memory
- Try closing other applications
- Restart the program

**3. "Invalid contact data on line X, skipping"**
- CSV file has corrupted data
- Check the CSV file format
- Remove problematic lines manually

**4. Search returns "not found" but contact exists**
- Check spelling and case for name searches
- Ensure exact match for phone and roll number
- Use "List All Contacts" to verify contact details

**5. Binary search gives different results than linear search**
- This is normal - binary search sorts data first
- Results are correct but order may change
- Use linear search if you want to maintain original order

**6. Cannot delete or edit contact**
- Ensure you enter a valid contact number
- Check that contacts exist (use "List All Contacts")
- Numbers are assigned sequentially starting from 1

### File Issues

**Backup files accumulating**
- Automatic backups are created each time you save
- Safe to delete old backup files manually
- Format: `contacts.csv.backup_YYYYMMDD_HHMMSS`

**CSV file corruption**
- Keep backups of working CSV files
- Manually edit CSV file to fix format issues
- Use simple text editor (not Excel) for manual editing

### Performance Tips

1. **For large datasets (>1000 contacts)**:
   - Use binary search when possible
   - Save frequently to avoid data loss
   - Consider splitting into multiple files

2. **For frequent searches**:
   - Keep data sorted by most-searched field
   - Use binary search for name/roll number
   - Department search is always linear

3. **Memory efficiency**:
   - Application starts with capacity for 10 contacts
   - Automatically expands as needed
   - Memory is freed when program exits

### Getting Help

1. **In-application help**: Use menu option 9
2. **About information**: Use menu option 10
3. **Source code**: Check comments in .c files
4. **This document**: Comprehensive usage guide

Remember: The application is designed to be user-friendly with clear prompts and error messages. If you encounter issues, read the error messages carefully as they usually explain what went wrong and how to fix it.