# Files to compile as part of the project
OBJS = main.c

# Compiler
CC = gcc

# Compiler flags
COMPILER_FLAGS = -w

# Linker flags
LINKER_FLAGS = -lSDL2

# Executable output name
TARGET = gamebryo

# Target that compiles the executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)
