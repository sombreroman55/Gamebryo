# Files to compile as part of the project
SRCS = ./src/*.cc

# Include files
INCLUDES = -I./includes

# Compiler
CC = g++

# Compiler flags
COMPILER_FLAGS = -Wall

# Linker flags
LINKER_FLAGS = -lglfw -lGL -lX11 -lpthread -lXi -ldl -lm

# Executable output name
TARGET = gamebryo

# ImGui files
GSRCS = ./imgui/imgui*.cpp ./imgui/backends/imgui_impl_glfw.cpp ./imgui/backends/imgui_impl_opengl3.cpp
GINC = -I./imgui -I./imgui/backends

# Target that compiles the executable
all : $(SRCS)
	$(CC) $(INCLUDES) $(GINC) $(SRCS) $(GSRCS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TARGET)
