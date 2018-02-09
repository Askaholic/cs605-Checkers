// search.cpp
// Rohan Weeden
// Created: Feb. 9, 2018

// Implementation for searching functions

#include "board_funcs.h"
#include <memory>
#include <utility>

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

    std::vector<std::pair<std::unique_ptr<BoardState>, int>> board_results;
    std::pair<std::unique_ptr<BoardState>, int> best;
    for (size_t i = 0; i < next_boards.size(); i++) {
        board_results.push_back(
            min_max_search(
                next_boards[i],
                player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER,
                depth - 1
            )
        );
        if (i == 0) {
            // TODO: Is this an extra copy?
            best = board_results[0];
        }
        else if (board_results[i].second > best.second) {
            best = board_results[i];
        }
    }

    return best;
}
