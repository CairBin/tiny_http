# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
LIB_DIR := lib
BIN_DIR := bin

# Output library and executable
LIB_TARGET := $(LIB_DIR)/libtiny_http.a
APP_TARGET := $(BIN_DIR)/app

# Source and object files for library
LIB_SOURCES := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_SOURCES))

# Source and object files for app
APP_SOURCES := $(SRC_DIR)/main.cpp
APP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(APP_SOURCES))

# Default target
all: $(LIB_TARGET) $(APP_TARGET)

# Create static library
$(LIB_TARGET): $(LIB_OBJECTS)
	$(if $(filter Windows_NT,$(OS)),if not exist $(LIB_DIR) mkdir $(LIB_DIR),mkdir -p $(LIB_DIR))
	ar rcs $@ $^

# Create app executable
$(APP_TARGET): $(APP_OBJECTS) $(LIB_TARGET)
	$(if $(filter Windows_NT,$(OS)),if not exist $(BIN_DIR) mkdir $(BIN_DIR),mkdir -p $(BIN_DIR))
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(LIB_DIR) -ltiny_http $(if $(filter Windows_NT,$(OS)), -lwsock32)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(if $(filter Windows_NT,$(OS)),if not exist $(OBJ_DIR) mkdir $(OBJ_DIR),mkdir -p $(OBJ_DIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	$(if $(filter Windows_NT,$(OS)),rmdir /S /Q $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR),rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR))

# Rebuild the project
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild