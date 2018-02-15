// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H

#include <cstddef>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>

#define RED_PLAYER 0
#define BLACK_PLAYER 1

#define BLANK 0x00
#define BLACK_CHECKER 0x01
#define RED_CHECKER 0x02
#define BLACK_KING 0x03
#define RED_KING 0x04

#define BOARD_ELEMENTS 32

struct Move {
    size_t _from;
    size_t _to;
};

struct Jump {
    size_t _from;
    size_t _to;
    size_t _enemy;
};

class BoardState {
private:
    char _tiles[BOARD_ELEMENTS / 2];

public:
    BoardState() = default;
    BoardState(const BoardState &);
    char operator[](size_t index);
    const char operator[](size_t index) const;
    void set(size_t index, char value);
    // test timing with reference?
    void apply_move(const Move);
    void apply_jump(const Jump);
};

class BoardStateFast {
private:
    char _tiles[BOARD_ELEMENTS];

public:
    char operator[](size_t index);
    void set(size_t index, char value);
};

// Char * manipulation functions
char board_get_one(const char * start, size_t i);
void board_set_one(char * start, size_t i, char val);
void board_write(char * start, const BoardState & board);
void board_read(const char * start, BoardState & board);
void board_copy(const char * from, char * to);
void board_apply_move(char * start, Move move);


class Board {
private:
    BoardState currentBoardState;
public:
};

// Test function. To verify that the module is installed and works
char * test(char * str);

void setup_board();
void setup_network();
BoardState get_board(); 

std::vector<Move> get_possible_moves(const BoardState &board, int player);
std::vector<Move> get_possible_moves(const char * board, int player);

std::vector<Jump> get_possible_jumps(const BoardState &board, int player);
std::vector<Jump> get_possible_jumps(const char * board, int player);

float evaluate_board(const BoardState &board);
int piece_count(const BoardState &board, int player);
int piece_count(const char *, int player);
std::pair<std::unique_ptr<BoardState>, int> min_max_search(const BoardState & board, int player, int depth);
std::pair<BoardState, int> min_max_no_alloc(const BoardState & board, int player, int depth);

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
