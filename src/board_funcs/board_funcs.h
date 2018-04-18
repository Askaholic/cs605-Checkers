// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H

#include "board_state.h"
#include <vector>


void setup_network(int playerColor);
BoardState make_move(const BoardState & board);

std::vector<Move> get_possible_moves(const BoardState &board, int player);
bool is_valid_jump(const BoardState & board, const Jump & jump, int player);


BoardState make_board(const std::vector<char> & board);

void print_board(const BoardState & board);
void print_board(const BoardState & board, std::string linePrefix);





void time_boards();


bool is_valid_index(const int index);



#endif
