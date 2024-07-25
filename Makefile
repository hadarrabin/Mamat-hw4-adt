# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -std=c99 -fpic
LDFLAGS = -shared

# Object files and library name
OBJS = grades.o
LIB = libgrades.so

# Phony targets
.PHONY: all clean

# Default target
all: $(LIB)

# Shared library
$(LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -llinked-list -L.

# Object files
%.o: %.c grades.h linked-list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	$(RM) $(OBJS) $(LIB)