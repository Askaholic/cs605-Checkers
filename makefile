# Compiler options
CC=g++
CFLAGS= -c -Wall --std=c++14 -mavx -O2 \
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
	src/board/moves/move_table.cpp \
	src/board/moves/jump_table.cpp \
	src/board/moves/jump_generator.cpp \
	src/game/game.cpp \
	src/game/player.cpp \
	src/board_funcs/helpers.cpp \
	src/search/search.cpp \
	src/asknet/asknet.cpp \
	src/asknet/asknet4.cpp

MAIN_FILES=\
	test/test.cpp \
	test/catch_main.cpp \
	main/game_test.cpp \
	main/network_test.cpp \
	main/search_benchmark.cpp \
	main/train.cpp

TEST_EXECUTABLE=test
NET_TEST_EXECUTABLE=nettest
GAME_EXECUTABLE=game
TRAIN_EXECUTABLE=train
SEARCH_EXECUTABLE=searchtest

EXECUTABLE_FILES  = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES      = $(SOURCE_FILES:%.cpp=$(OBJ)/%.o)
OBJECT_FILES_MAIN = $(MAIN_FILES:%.cpp=$(OBJ)/%.o)

all: $(TEST_EXECUTABLE) $(NET_TEST_EXECUTABLE) $(GAME_EXECUTABLE) $(TRAIN_EXECUTABLE) $(SEARCH_EXECUTABLE)

# Make executables
$(TEST_EXECUTABLE): $(BIN)/$(TEST_EXECUTABLE)
$(NET_TEST_EXECUTABLE): $(BIN)/$(NET_TEST_EXECUTABLE)
$(GAME_EXECUTABLE): $(BIN)/$(GAME_EXECUTABLE)
$(TRAIN_EXECUTABLE): $(BIN)/$(TRAIN_EXECUTABLE)
$(SEARCH_EXECUTABLE): $(BIN)/$(SEARCH_EXECUTABLE)

# Link object files
$(BIN)/$(SEARCH_EXECUTABLE): $(OBJ)/main/search_benchmark.o $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"

$(BIN)/$(TEST_EXECUTABLE): $(OBJ)/test/catch_main.o $(OBJECT_FILES) $(OBJ)/test/test.o
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"


$(BIN)/$(NET_TEST_EXECUTABLE): $(OBJ)/main/network_test.o $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"


$(BIN)/$(GAME_EXECUTABLE): $(OBJ)/main/game_test.o $(OBJECT_FILES)
	@echo Building $^
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build Successful"


$(BIN)/$(TRAIN_EXECUTABLE): $(OBJECT_FILES) $(OBJ)/main/train.o
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

.PHONY: clean all
