// jump_generator.cpp
// Rohan Weeden
// Created: Feb. 21, 2018

// Jump generator implementation

#include "board_funcs.h"
#include "jump_table.h"
#include <vector>


void set_pieces_for_player(char & checker, char & king, int player) {
    if (player == RED_PLAYER) {
        checker = RED_CHECKER;
        king = RED_KING;
    }
    else if (player == BLACK_PLAYER) {
        checker = BLACK_CHECKER;
        king = BLACK_KING;
    }
}

void adjust_jump_indecies_for_piece(size_t & start, size_t & end, char current_piece) {
    // Grab only the north direction
    if (current_piece == BLACK_CHECKER) {
        end -= 2;
    }
    // grab only the south direction
    else if (current_piece == RED_CHECKER) {
        start += 2;
    }
}

// Appends any successive jumps if there are any, otherwise just appends board.
void JumpGenerator::_append_jumps_for_piece(std::vector<BoardState> & jumps, const BoardState & board, size_t index) {
    if (!is_valid_index(index)) {
        throw std::out_of_range("Bad index passed to _append_jumps_for_piece");
    }
    auto piece = board[index];

    auto jump_locations = jumpTable[index];
    size_t start = 0;
    size_t end = jump_locations.size();
    adjust_jump_indecies_for_piece(start, end, piece);

    bool has_another_jump = false;
    bool is_king = piece == _king;

    for (size_t i = 0; i < jump_locations.size(); i++) {
        if (jump_locations[i][0] == -1) { continue; }

        Jump possible_jump = {index, jump_locations[i][0], jump_locations[i][1]};
        if (is_valid_jump(board, possible_jump, _player)) {
            has_another_jump = true;

            BoardState next_board = BoardState(board);
            next_board.apply_jump(possible_jump);
            size_t next_index = possible_jump._to;

            // When the checker becomes a king it must stop jumping
            if (!is_king && next_board[next_index] == _king) {
                jumps.emplace_back(next_board);
            }
            else {
                _append_jumps_for_piece(jumps, next_board, next_index);
            }
        }
    }
    if (!has_another_jump) {
        jumps.emplace_back(board);
    }
}

std::vector<BoardState> JumpGenerator::get_possible_jumps(const BoardState & board, int player) {
    std::vector<BoardState> jumps;
    _player = player;
    set_pieces_for_player(_checker, _king, _player);

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {
        auto piece = board[b_loc];
        // Skip squares that don't have the correct color checker
        if (piece != _checker && piece != _king) {
            continue;
        }

        _append_jumps_for_piece(jumps, board, b_loc);
    }

    return jumps;
}
