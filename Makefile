# Compiler
CXX = g++

# Compiler flags: Use C++17 standard, enable warnings, add debug info
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Include flags: Point to the directory containing the SDL2 folder
CPPFLAGS = -I/opt/homebrew/include/SDL2

# Linker flags: Use sdl2-config to get necessary library paths and linking flags for SDL2
LDFLAGS = $(shell sdl2-config --libs)

# Source files
# --- Added InterruptHandler.cpp ---
SRCS = main.cpp cpu.cpp gb.cpp input.cpp mmap.cpp mmu.cpp ppu.cpp RAM.cpp InterruptHandler.cpp

# Object directory
OBJDIR = obj

# Object files (derived from source files, placed in OBJDIR)
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

# Executable name
TARGET = gheithboy

# Default target: Build the executable
all: $(TARGET)

# Rule to link the executable from object files
$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Generic rule to compile source files (.cpp) into object files (.o) in OBJDIR
# $< represents the first prerequisite (the .cpp file)
# $@ represents the target (the .o file in OBJDIR)
$(OBJDIR)/%.o: %.cpp *.hpp # Added *.hpp dependency to recompile if headers change
	@echo "Compiling $< -> $@..."
	@mkdir -p $(OBJDIR) # Ensure the object directory exists
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule to clean up build artifacts (object files and the executable)
clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJDIR) $(TARGET) # Remove the object directory and the target

# Declare 'all' and 'clean' as phony targets, meaning they aren't actual files
.PHONY: all clean
