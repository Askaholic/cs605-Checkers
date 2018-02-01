// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include <vector>
#include <cstddef>
#include "board_funcs.h"

std::vector<std::vector<int>> moveTable = {
    {-1, -1,		-1, 4},
    {-1, -1,		4, 5},
    {-1, -1,		5, 6},
    {-1, -1,		6, 7},

    {0, 1,		8, 9},
    {1, 2,		9, 10},
    {2, 3,		10, 11},
    {3, -1,		11, -1},

    {-1, 4,		-1, 12},
    {4, 5,		12, 13},
    {5, 6,		13, 14},
    {6, 7,		14, 15},

    {8, 9,		16, 17},
    {9, 10,		17, 18},
    {10, 11,		18, 19},
    {11, -1,		19, -1},

    {-1, 12,		-1, 20},
    {12, 13,		20, 21},
    {13, 14,		21, 22},
    {14, 15,		22, 23},

    {16, 17,		24, 25},
    {17, 18,		25, 26},
    {18, 19,		26, 27},
    {19, -1,		27, -1},

    {-1, 20,		-1, 28},
    {20, 21,		28, 29},
    {21, 22,		29, 30},
    {22, 23,		30, 31},

    {24, 25,		-1, -1},
    {25, 26,		-1, -1},
    {26, 27, 	-1, -1},
    {27, -1,		-1, -1}
};
BoardState the_board = {};

void setup_board() {
    for (size_t i = 0; i < 12; i++) {
        the_board.set(i, RED_CHECKER);
        the_board.set(BOARD_ELEMENTS - 1 - i, BLACK_CHECKER);
    }
}

BoardState get_board() {
  return the_board;
}

std::vector<Move> get_possible_moves(const BoardState &board, int player) {
    std::vector<Move> moves;

    char targets[2];
    if (player == RED_PLAYER) {
        targets[0] = RED_CHECKER;
        targets[1] = RED_KING;
    }
    else {
        targets[0] = BLACK_CHECKER;
        targets[1] = BLACK_KING;
    }


    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        if (!in_<char>(targets, 2, board[b_loc])) {
            continue;
        }

        auto possible_moves = moveTable[b_loc];
        for (size_t c = 0; c < possible_moves.size(); c++) {
            size_t move = possible_moves[c];
            if (board[move] == BLANK) {
                moves.push_back({b_loc, move});
            }
        }
    }

    return moves;
}
