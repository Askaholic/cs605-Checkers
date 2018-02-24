// piece_count.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Implementation of piece count functions

#include "piece_count.h"
#include "board_state.h"
#include <cstddef>


float piece_count(const BoardState &board, int player) {
    char targets[2];

    float piece_val = 1;
    float king_val = 1.5;

    const char piece = player == RED_PLAYER ? RED_CHECKER : BLACK_CHECKER;
    const char king = player == RED_PLAYER ? RED_KING : BLACK_KING;

    const char enemy_piece = player == RED_PLAYER ? BLACK_CHECKER : RED_CHECKER;
    const char enemy_king = player == RED_PLAYER ? BLACK_KING : RED_KING;

    float count = 0;

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        auto current_piece = board[b_loc];
        if (current_piece == piece) {
            count += piece_val;
        }
        else if (current_piece == king) {
            count += king_val;
        }
        else if (current_piece == enemy_piece) {
            count -= piece_val;
        }
        else if (current_piece == enemy_king) {
            count -= king_val;
        }
    }

    return count;
}
