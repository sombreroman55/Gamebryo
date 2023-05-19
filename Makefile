# Files to compile as part of the project
SRCS = main.c cpu.c mem.c system.c

# Include files
INCLUDES = -I.

# Compiler
CC = g++

# Compiler flags
COMPILER_FLAGS = -Wall

# Linker flags
LINKER_FLAGS = -lglfw -lGL -lX11 -lpthread -lXi -ldl -lm -lSDL2

# Executable output name
TARGET = gamebryo

# Target that compiles the executable
all : $(SRCS)
	$(CC) $(INCLUDES) $(SRCS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)

# Target that cleans the project
clean :
	rm $(TARGET)
