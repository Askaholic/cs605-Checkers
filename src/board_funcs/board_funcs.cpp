// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include "asknet.h"
#include "asknet4.h"
#include "board_funcs.h"
#include "jump_table.h"
#include <vector>
#include <cstddef>
#include <iostream>


std::vector<std::vector<int>> moveTable = {
    {-1,-1,   4,5},
    {-1,-1,   5,6},
    {-1,-1,   6,7},
    {-1,-1,  -1,7},

    {-1,0,   -1,8},
    {0,1,     8,9},
    {1,2,     9,10},
    {2,3,     10,11},

    {4,5,    12,13},
    {5,6,    13,14},
    {6,7,    14,15},
    {7,-1,   15,-1},

    {-1,8,  -1,16},
    {8,9,    16,17},
    {9,10,   17,18},
    {10,11,  18,19},

    {12,13,  20,21},
    {13,14,  21,22},
    {14,15,  22,23},
    {15,-1,  23,-1},

    {-1,16,  -1,24},
    {16,17,  24,25},
    {17,18,  25,26},
    {18,19,  26,27},

    {20,21,  28,29},
    {21,22,  29,30},
    {22,23,  30,31},
    {23,-1,  31,-1},

    {-1,24,  -1,-1},
    {24,25,  -1,-1},
    {25,26,  -1,-1},
    {26,27,  -1,-1}
};



BoardState the_board = {};
Network the_network({0, 0});


void setup_board() {
    for (size_t i = 0; i < 12; i++) {
        the_board.set(i, RED_CHECKER);
        the_board.set(BOARD_ELEMENTS - 1 - i, BLACK_CHECKER);
    }
}

void setup_network() {
    the_network = Network({32, 1024, 512, 256, 8, 1});
    the_network.randomizeWeights();
    std::cout << "Initialized network with " << the_network.getNumNodes() << " nodes" << '\n';
}


BoardState get_board() {
  return the_board;
}

float evaluate_board(const BoardState &board) {
    std::vector<float> inputs(BOARD_ELEMENTS);

    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        auto piece = board[i];
        float node_value = 0.0f;
        switch (piece) {
            case RED_CHECKER: node_value = 0.5f; break;
            case RED_KING: node_value = 1.0f; break;
            case BLACK_CHECKER: node_value = -0.5f; break;
            case BLACK_KING: node_value = -1.0f; break;
        }
        inputs[i] = node_value;
    }

    return the_network.evaluate(inputs);
}

void _set_targets(char * targets, int player) {
    if (player == RED_PLAYER) {
        targets[0] = RED_CHECKER;
        targets[1] = RED_KING;
    }
    else {
        targets[0] = BLACK_CHECKER;
        targets[1] = BLACK_KING;
    }
}


std::vector<Move> get_possible_moves(const BoardState &board, int player) {
    std::vector<Move> moves;

    char targets[2];
    _set_targets(targets, player);

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {

        auto current_piece = board[b_loc];
        // Skip squares that don't have the correct color checker
        if (!in_<char>(targets, 2, current_piece)) {
            continue;
        }

        auto possible_moves = moveTable[b_loc];
        size_t start = 0;
        size_t end = possible_moves.size();

        // Grab only the north direction
        if (current_piece == BLACK_CHECKER) {
            end -= 2;
        }
        // grab only the south direction
        else if (current_piece == RED_CHECKER) {
            start += 2;
        }

        for (size_t c = start; c < end; c++) {
            auto move = possible_moves[c];

            if (move == -1) { continue; }

            if (board[move] == BLANK) {
                moves.push_back({b_loc, move});
            }
        }
    }

    return moves;
}


bool is_valid_jump(const BoardState & board, const Jump & jump, int player) {

    if (board[jump._to] != BLANK){
       return false;
    }

    char targets[2];
    _set_targets(targets, player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER);

    if (!in_<char>(targets, 2, board[jump._enemy]) ){
        return false;
    }

    return true;
}

void get_possible_jumps_for_piece(std::vector<Jump> & jumps, const BoardState & board, size_t index, int player) {
    auto current_piece = board[index];
    auto possible_jump = jumpTable[index];
    size_t start = 0;
    size_t end = possible_jump.size();

    // Grab only the north direction
    if (current_piece == BLACK_CHECKER) {
        end -= 2;
    }
    // grab only the south direction
    else if (current_piece == RED_CHECKER) {
        start += 2;
    }

    for (auto ii = start; ii < end; ii++) {
        auto jump = possible_jump[ii];
        if (jump[0] == -1) { continue; }

        Jump poss_jump = {index, jump[0], jump[1]};

        if (is_valid_jump(board, poss_jump, player)) {
            jumps.push_back(poss_jump);
        }
    }
}

std::vector<Jump> get_possible_jumps(const BoardState &board, int player) {
    std::vector<Jump> jumps;
    char targets[2];
    _set_targets(targets, player);

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        auto current_piece = board[b_loc];
        // Skip squares that don't have the correct color checker
        if (!in_<char>(targets, 2, current_piece)) {
            continue;
        }

        get_possible_jumps_for_piece(jumps, board, b_loc, player);
    }

    return jumps;
}
