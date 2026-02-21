# Compiler and Linker
CC       := gcc
CXX      := g++

# Target executable name
TARGET   := giteapc-installer

# Flags
CFLAGS   := -Wall -Wextra -g
CXXFLAGS := -Wall -Wextra -g -std=c++17
LDFLAGS  := -lncurses

# Find all .c and .cpp files under src/ directory
SRCS_C   := $(wildcard src/*.c)
SRCS_CPP := $(wildcard src/*.cpp)

# Generate object file names from source file names
OBJS     := $(SRCS_C:.c=.o) $(SRCS_CPP:.cpp=.o)

# Default rule: build the target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule for C object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for C++ object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
