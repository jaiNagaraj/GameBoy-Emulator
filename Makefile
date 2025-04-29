# Compiler
CXX = g++

# ROM Path Variable (default can be anything or empty, ?= allows override)
# This is now primarily used by the 'run' target for convenience.
# The executable itself expects the ROM name as a command-line argument.
ROM ?= tetris.gb

# Compiler flags: Use C++17 standard, enable warnings, add debug info
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Include flags: Point to the project's include directory and SDL2 include directory
CPPFLAGS = -Iinclude -I/opt/homebrew/include/SDL2

# Linker flags: Use sdl2-config to get necessary library paths and linking flags for SDL2
LDFLAGS = $(shell sdl2-config --libs)

# Source directory
SRCDIR = src

# Source files (find all .cpp files in SRCDIR)
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Object directory
OBJDIR = obj

# Object files (derived from source files, placed in OBJDIR)
# Replaces src/ prefix with obj/ and changes .cpp to .o
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Header files (find all .hpp files in include/)
HDRS = $(wildcard include/*.hpp)

# Executable name
TARGET = gheithboy

# Default target: Build the executable
all: $(TARGET)

# Rule to link the executable from object files
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Generic rule to compile source files (.cpp) into object files (.o) in OBJDIR
# Depends on the corresponding .cpp file and all header files.
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRS) Makefile
	@echo "Compiling $< -> $@..."
	@mkdir -p $(OBJDIR) # Ensure the object directory exists
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Target to run the executable with a specified ROM (passed as argument)
# Example: make run ROM=dr_mario.gb
run: $(TARGET)
	@echo "Running $(TARGET) with ROM: $(ROM)..."
	./$(TARGET) $(ROM) # Assumes main.cpp handles the ./games/ prefix

# Rule to clean up build artifacts (object files and the executable)
clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJDIR) $(TARGET) # Remove the object directory and the target

# Declare 'all', 'clean', and 'run' as phony targets, meaning they aren't actual files
.PHONY: all clean run
