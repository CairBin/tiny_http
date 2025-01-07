# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Output binary
TARGET := $(BIN_DIR)/app

# Source and object files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Create binary
$(TARGET): $(OBJECTS)
	$(if $(filter Windows_NT,$(OS)),if not exist $(BIN_DIR) mkdir $(BIN_DIR),mkdir -p $(BIN_DIR))
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(if $(filter Windows_NT,$(OS)),if not exist $(OBJ_DIR) mkdir $(OBJ_DIR),mkdir -p $(OBJ_DIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	$(if $(filter Windows_NT,$(OS)),rmdir /S /Q $(OBJ_DIR) $(BIN_DIR),rm -rf $(OBJ_DIR) $(BIN_DIR))

# Rebuild the project
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild