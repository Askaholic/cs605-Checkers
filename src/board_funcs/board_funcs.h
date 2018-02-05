// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H

#include <cstddef>
#include <vector>
#include <iostream>

#define RED_PLAYER 0
#define BLACK_PLAYER 1

#define BLANK 0x00
#define BLACK_CHECKER 0x01
#define RED_CHECKER 0x02
#define BLACK_KING 0x03
#define RED_KING 0x04

#define BOARD_ELEMENTS 32

class BoardState {
private:
    char _tiles[BOARD_ELEMENTS / 2];

public:
    char operator[](size_t index);
    const char operator[](size_t index) const;
    void set(size_t index, char value);
};

class BoardStateFast {
private:
    char _tiles[BOARD_ELEMENTS];

public:
    char operator[](size_t index);
    void set(size_t index, char value);
};

class Board {
private:
    BoardState currentBoardState;

public:
};

struct Move {
    size_t _from;
    size_t _to;
};

// Test function. To verify that the module is installed and works
char * test(char * str);

void setup_board();
BoardState get_board();
std::vector<Move> get_possible_moves(const BoardState &board, int player);
void time_boards();

template <typename T>
bool in_(const T * array, size_t length, const T &value) {
    for (size_t i = 0; i < length; i++) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
};

#endif
