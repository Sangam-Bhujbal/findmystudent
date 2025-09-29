# Makefile for Address Book Project

# Compiler settings
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g
TARGET = addressbook
TARGET_WIN = addressbook.exe

# Source files
SOURCES = main.c contact.c file.c populate.c
HEADERS = contact.h file.h populate.h

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build target for Unix-like systems
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Build target for Windows
windows: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET_WIN) $(OBJECTS)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET_WIN)
	rm -f *.backup_*
	rm -f contacts.csv.backup_*

# Clean and rebuild
rebuild: clean all

# Install dependencies (for development)
install-dev:
	@echo "Installing development dependencies..."
	@echo "For Ubuntu/Debian: sudo apt-get install gcc make valgrind cppcheck"
	@echo "For macOS: xcode-select --install or install via Homebrew"
	@echo "For Windows: Install MinGW-w64 or Visual Studio"

# Run static analysis
analyze:
	@echo "Running static analysis..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h; \
	else \
		echo "cppcheck not found. Install it for static analysis."; \
	fi

# Memory check with valgrind (Linux/macOS only)
memcheck: $(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Running memory check with valgrind..."; \
		echo -e "10\n0\nn" | valgrind --leak-check=full --track-origins=yes ./$(TARGET); \
	else \
		echo "valgrind not found. Available on Linux/macOS only."; \
	fi

# Run basic functionality test
test: $(TARGET)
	@echo "Running basic functionality tests..."
	@echo "Testing help menu:"
	@echo "9" | timeout 5s ./$(TARGET) || true
	@echo -e "\nTesting about menu:"
	@echo -e "10\n0\nn" | timeout 5s ./$(TARGET) || true
	@echo "Basic tests completed."

# Package project for distribution
package: clean
	@echo "Creating distribution package..."
	@mkdir -p dist/address-book
	@cp *.c *.h *.md *.csv Makefile dist/address-book/
	@cp -r .github dist/address-book/ 2>/dev/null || true
	@cd dist && tar -czf address-book.tar.gz address-book/
	@echo "Package created: dist/address-book.tar.gz"

# Show project information
info:
	@echo "Address Book Management System"
	@echo "=============================="
	@echo "Language: C (C99 standard)"
	@echo "Files: $(SOURCES) $(HEADERS)"
	@echo "Compiler: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build for Unix-like systems"
	@echo "  windows   - Build for Windows"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and rebuild"
	@echo "  analyze   - Run static analysis"
	@echo "  memcheck  - Run memory check (requires valgrind)"
	@echo "  test      - Run basic functionality tests"
	@echo "  package   - Create distribution package"
	@echo "  info      - Show this information"

# Phony targets
.PHONY: all clean rebuild install-dev analyze memcheck test package info windows

# Default goal
.DEFAULT_GOAL := all