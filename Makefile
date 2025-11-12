# Compiler and compiler flags
CC = gcc
CFLAGS = -g -std=c11 -Wall -Wextra -Wshadow
LDFLAGS = -lm -lpng -ljpeg

# Include paths (adjust for your system)
INCLUDES = -I/ucrt64/include
# For Linux/macOS with standard paths, comment out INCLUDES or set to empty:
# INCLUDES =

# Source files and executable name
SRCS = filter.c helpers.c image_io.c bmp_io.c png_io.c jpeg_io.c
TARGET = filter.exe

# Default target
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

# Rule to clean up generated files
clean:
	rm -f $(TARGET) *.o
