# Makefile for the project

# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE
LDFLAGS := -lncurses
# Source files
SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.c)

# Object files
OBJDIR := build
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Executable name
EXEC := bomberman

# Build rules
all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXEC)
