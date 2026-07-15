# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -O3 -pthread

# The output executable name
TARGET = test_allocator

# The source file(s)
SRC = test.c

# Default rule when you just type 'make'
all:  $(TARGET)

# How to build the target
$(TARGET): $(SRC)
  $(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up compiled files
clean:
  rm -f $(TARGET)
