# Compiler options
CC=g++
CFLAGS=-c -Wall --std=c++14 -mavx -O2 \
		-Isrc/board_funcs -Isrc/asknet \
 		-Isrc/board -I src/board/moves -Isrc/search \
		-Isrc/game
LDFLAGS=

# Folders
BIN=build
OBJ=$(BIN)/obj

# Files
SOURCE_FILES=\
	src/board_funcs/board_funcs.cpp \
	src/board/board_state.cpp \
	src/board/piece_count.cpp \
	src/board/moves/jump_table.cpp \
	src/board/moves/jump_generator.cpp \
	src/game/game.cpp \
	src/board_funcs/helpers.cpp \
	src/search/little_search.cpp \
	src/asknet/asknet.cpp

MAIN_FILES=\
	test/catch_main.cpp \
	main/game_test.cpp \
	main/train.cpp

TEST_EXECUTABLE=test
GAME_EXECUTABLE=game
TRAIN_EXECUTABLE=train

EXECUTABLE_FILES  = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES      = $(SOURCE_FILES:%.cpp=$(OBJ)/%.o)
OBJECT_FILES_MAIN = $(MAIN_FILES:%.cpp=$(OBJ)/%.o)

all: $(TEST_EXECUTABLE) $(GAME_EXECUTABLE) $(TRAIN_EXECUTABLE)

# Make executables
$(TEST_EXECUTABLE): $(BIN)/$(TEST_EXECUTABLE)
$(GAME_EXECUTABLE): $(BIN)/$(GAME_EXECUTABLE)
$(TRAIN_EXECUTABLE): $(BIN)/$(TRAIN_EXECUTABLE)

# Link object files
$(BIN)/$(TEST_EXECUTABLE): $(OBJ)/test/catch_main.o $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"


$(BIN)/$(GAME_EXECUTABLE): $(OBJ)/main/game_test.o $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"


$(BIN)/$(TRAIN_EXECUTABLE): $(OBJ)/main/train.o $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"

# Compile source files
$(OBJECT_FILES) $(OBJECT_FILES_MAIN) : $(OBJ)/%.o: %.cpp
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -rf $(BIN)

.PHONY: $(TEST_EXECUTABLE) $(GAME_EXECUTABLE) $(TRAIN_EXECUTABLE) clean all
