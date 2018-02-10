// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include "asknet.h"
#include "board_funcs.h"
#include <vector>
#include <cstddef>
#include <iostream>

/*
jumpTable = {
	 	[-1,-1,        -1,(9,5)],
	 	[-1,-1,		(8,5),(10,6)],
	 	[-1,-1,		(9,6),(11,7)],
	 	[-1,-1,		(10,7),-1],

	 	[-1,-1,		-1,(13,8)],
	 	[-1,-1,		(12,8),(14,9)],
	 	[-1,-1,		(13,9),(15,10)],
	 	[-1,-1,		(14,10),-1],

	 	[-1,(1,5),		    -1,(17,13)],
	 	[(0,5),(2,6),		(16,13),(18,14)],
	 [(1,6),(3,7),	     (17,14),(19,15)],
	 [(2,7),-1,		      (18,15),-1],

	 [-1,(5,8),		-1,(21,16)],
	 [(4,8),(6,9),		(20,16),(22,17)],
	 [(5,9),(7,10),		(21,17),(23,18)],
	 [(6,10),-1,		(22,18),-1],

	 [-1,(9, 13),		-1,(25, 21)],
	 [(8, 13),(10, 14),		(24, 21),(26, 22)],
	 [(9, 14),(11, 15),		(25, 22),(27, 23)],
	 [(10, 15),-1,		(26, 23),-1],

	 [-1,(13, 16),		-1,(29, 24)],
	 [(12, 16),(14, 17),		(28, 24),(30, 25)],
		[(13, 17),(15, 18),		(29, 25), (31, 26)],
	 [(14, 18),-1,		(30, 26),-1],

		[(17, 21),-1,		-1,-1],
		[(16, 21),(18, 22),		-1,-1],
	 [(17, 22),(19, 23),		-1,-1],
	 [(18, 23),-1,		-1,-1],

	[-1,(21, 24),		-1,-1],
	[(20, 24),(22, 25),		-1,-1],
	[(21, 25),(23, 26),		-1,-1],
	[(22, 26),-1,		-1,-1],

}
*/

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

int piece_count(const BoardState &board, int player) {
    char targets[2];
    _set_targets(targets, player);
    size_t count = 0;

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        auto current_piece = board[b_loc];
        // Skip squares that don't have the correct color checker
        if (!in_<char>(targets, 2, current_piece)) {
            continue;
        }
        count++;
    }

    return count;
}

int piece_count(const char * start, int player) {
    char targets[2];
    _set_targets(targets, player);
    size_t count = 0;

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        auto current_piece = board_get_one(start, b_loc);
        // Skip squares that don't have the correct color checker
        if (!in_<char>(targets, 2, current_piece)) {
            continue;
        }
        count++;
    }

    return count;
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

std::vector<Move> get_possible_moves(const char * board, int player) {
    std::vector<Move> moves;

    char targets[2];
    _set_targets(targets, player);

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        auto current_piece = board_get_one(board, b_loc);
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

            if (board_get_one(board, move) == BLANK) {
                moves.push_back({b_loc, move});
            }
        }
    }

    return moves;
}
