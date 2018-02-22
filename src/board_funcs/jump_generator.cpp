// jump_generator.cpp
// Rohan Weeden
// Created: Feb. 21, 2018

// Jump generator implementation

#include "board_funcs.h"
#include "jump_table.h"
#include <vector>

// Appends any successive jumps if there are any, otherwise just appends board.
void JumpGenerator::_append_jumps_for_piece(std::vector<BoardState> & jumps, const BoardState & board, size_t index, int player) {
    if (!is_valid_index(index)) {
        throw std::out_of_range("Bad index passed to _append_jumps_for_piece");
    }

    char checker, king;

    if (player == RED_PLAYER) {
        checker = RED_CHECKER;
        king = RED_KING;
    }
    else if (player == BLACK_PLAYER) {
        checker = BLACK_CHECKER;
        king = BLACK_KING;
    }

    auto piece = board[index];

    if (piece != checker && piece != king) {
        return;
    }

    auto jump_locations = jumpTable[index];
    bool has_another_jump = false;
    bool is_king = piece == king;
    for (size_t i = 0; i < jump_locations.size(); i++) {
        Jump possible_jump = {index, jump_locations[0][0], jump_locations[0][1]};
        if (is_valid_jump(board, possible_jump, player)) {
            has_another_jump = true;

            BoardState next_board = BoardState(board);
            next_board.apply_jump(possible_jump);
            size_t next_index = possible_jump._to;

            // When the checker becomes a king it must stop jumping
            if (!is_king && next_board[next_index] == king) {
                jumps.emplace_back(next_board);
            }
            else {
                _append_jumps_for_piece(jumps, next_board, next_index, player);
            }
        }
    }
    if (!has_another_jump) {
        jumps.emplace_back(board);
    }
}

std::vector<BoardState> JumpGenerator::get_possible_jumps(const BoardState & board, int player) {

    return {};
}
