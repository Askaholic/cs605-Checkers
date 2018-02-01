// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H

#include <cstddef>
#include <vector>

#define BLANK 0x00
#define BLACK_CHECKER 0x01
#define RED_CHECKER 0x02
#define BLACK_KING 0x03
#define RED_KING 0x04

class BoardState {
public:
    char operator[](size_t index);
    void set(size_t index, char value);
private:
    char _tiles[16];
};

class BoardStateFast {
public:
    char operator[](size_t index);
    void set(size_t index, char value);
private:
    char _tiles[32];
};

// Test function. To verify that the module is installed and works
char * test(char * str);

void setup_board();
BoardState get_board();

std::vector<BoardState> get_possible_moves(BoardState board, int player);

void time_boards();

#endif
