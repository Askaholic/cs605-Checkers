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
#include <string>

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


const int red_king_end[] = {28, 29, 30, 31};
const int black_king_end[] = {12, 13, 14, 15};







class BoardState {
public:
    char _tiles[BOARD_ELEMENTS/2 ];

public:
    BoardState() = default;
    BoardState(const BoardState &);
    BoardState & operator=(const BoardState & other);
    bool operator==(const BoardState & other) const;
    char operator[](size_t index);
    const char operator[](size_t index) const;
    void set(size_t index, char value);
    // test timing with reference?
    void apply_move(const Move);
    void apply_jump(const Jump);
    void make_king(const int to);

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

class JumpGenerator {
private:
    /* data */
    void _append_jumps_for_piece(std::vector<BoardState> & jumps, const BoardState & board, size_t index, int player);

public:
    JumpGenerator () {}

    std::vector<BoardState> get_possible_jumps(const BoardState & board, int player);

    virtual ~JumpGenerator () {}
};

void setup_board();
void setup_network();
BoardState get_board();


std::vector<Move> get_possible_moves(const BoardState &board, int player);
std::vector<Jump> get_possible_jumps(const BoardState &board, int player);
void get_possible_jump_boards(std::vector<BoardState> & currJumps, const BoardState &board, int player);
bool is_valid_jump(const BoardState & board, const Jump & jump, int player);
std::pair<std::unique_ptr<BoardState>, int> min_max_search(const BoardState & board, int player, int depth);
std::pair<BoardState, int> min_max_search_ab(const BoardState & board, int player, int depth);
float evaluate_board(const BoardState &board);
float piece_count(const BoardState &board, int player);










std::pair<BoardState, float> min_max_search_inplace(const BoardState & board, int player, int depth);












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


bool is_valid_index(const int index);



#endif
