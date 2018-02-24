// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H

#include "search.h"
#include "board_state.h"
#include <cstddef>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>
#include <string>




class Board {
private:
    BoardState currentBoardState;
public:
};

class JumpGenerator {
private:
    /* data */
    char _checker;
    char _king;
    int _player;
    void _append_jumps_for_piece(std::vector<BoardState> & jumps, const BoardState & board, size_t index);
    void _do_multi_jump(std::vector<BoardState> & jumps, const BoardState & board, size_t index, std::vector<Jump> & table_jumps);
    bool _is_players_piece(char piece);


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
bool is_valid_jump(const BoardState & board, const Jump & jump, int player);
float evaluate_board(const BoardState &board);
float piece_count(const BoardState &board, int player);








void time_boards();


bool is_valid_index(const int index);



#endif
