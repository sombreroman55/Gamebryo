# Files to compile as part of the project
OBJS = mem.c cpu.c interrupt.c main.c

# Compiler
CC = gcc

# Compiler flags
COMPILER_FLAGS = -Wall

# Linker flags
LINKER_FLAGS = -lSDL2

# Executable output name
TARGET = gamebryo

# Target that compiles the executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)
