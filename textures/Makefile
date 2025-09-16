# Compiler
CC = g++

# Compiler flags (includes header directory)
CFLAGS = -Wall -g -I./include/

# Libraries to link
LIBS = -lglfw -lGL

# Source files
SRCS = main.cpp src/gl.c

# Name of the final executable
TARGET = a.out

# --- Rules ---

# Default rule: build the target
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Rule to clean up the build file
clean:
	rm -f $(TARGET)