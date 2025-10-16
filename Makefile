# Compiler and compiler flags
CC = gcc
CFLAGS = -g -std=c11 -Wall -Wextra -Werror -Wshadow
LDFLAGS = -lm

# Source files and executable name
SRCS = filter.c helpers.c
TARGET = filter

# Default target
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to clean up generated files
clean:
	rm -f $(TARGET)
