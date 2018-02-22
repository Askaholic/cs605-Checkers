# Compiler options
CC=g++
CFLAGS=-c -Wall --std=c++14 -Isrc/board_funcs -Isrc/board_funcs/asknet
LDFLAGS=
EXECUTABLE_NAME=test

# Folders
BIN=build
OBJ=$(BIN)/obj

# Files
SOURCE_FILES=\
	test/test.cpp \
	test/catch_main.cpp \
	src/board_funcs/board_funcs.cpp \
	src/board_funcs/board_state.cpp \
	src/board_funcs/helpers.cpp \
	src/board_funcs/jump_table.cpp \
	src/board_funcs/jump_generator.cpp \
	src/board_funcs/asknet/asknet.cpp


EXECUTABLE_FILES = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES     = $(SOURCE_FILES:%.cpp=$(OBJ)/%.o)

test: $(EXECUTABLE_FILES)

clean:
	rm -rf $(BIN)

.PHONY: test clean

$(EXECUTABLE_FILES): $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"

$(OBJECT_FILES): $(OBJ)/%.o: %.cpp
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $<
