// search.cpp
// Rohan Weeden
// Created: Feb. 9, 2018

// Implementation for searching functions

#include "board_funcs.h"
#include <memory>
#include <utility>

void min_max_search_helper(std::pair<std::unique_ptr<BoardState>, int> &, const BoardState & board, int player);

std::pair<std::unique_ptr<BoardState>, int> min_max_search(const BoardState & board, int player, int depth) {
    if (depth < 1) {
        auto result = std::make_pair(
            std::make_unique<BoardState>(
                BoardState(board)
            ),
            piece_count(board, player)
        );
        return result;
    }

    // Generate all possible next board states
    auto moves = get_possible_moves(board, player);
    std::vector<BoardState> next_boards;
    for (size_t i = 0; i < moves.size(); i++) {
        auto next = BoardState(board);
        next.apply_move(moves[i]);
        next_boards.push_back(next);
    }

    std::pair<std::unique_ptr<BoardState>, int> best;
    for (size_t i = 0; i < next_boards.size(); i++) {
        auto result = min_max_search(
            next_boards[i],
            player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER,
            depth - 1
        );
        if (i == 0 || result.second > best.second) {
            // TODO: Is this an extra copy?
            best = std::move(result);
            best.first = std::make_unique<BoardState>(next_boards[i]);
        }
    }

    return best;
}

void min_max_search_helper(std::pair<std::unique_ptr<BoardState>, int> &, const BoardState & board, int player) {

}
