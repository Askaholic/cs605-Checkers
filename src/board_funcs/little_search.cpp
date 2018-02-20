// little_search.cpp
// Rohan Weeden
// Created: Feb. 19, 2018

// Minimax search using in place alocation

#include "board_funcs.h"

#define BRANCH_FACTOR 10

struct SearchNode {
    BoardState board;
    float score;
    size_t best_score_index;
};

std::pair<BoardState, float> min_max_search_inplace(const BoardState & board, int player, int depth) {
    if (depth < 1) {
        return std::make_pair<BoardState, float>(
            BoardState(board),
            (float) piece_count(board, player)
        );
    }

    std::vector<SearchNode> search_mem((depth + 1) * BRANCH_FACTOR);
    search_mem[0].board = board;
    size_t indecies[depth] = {0};
    size_t moves_size[depth] = {0};
    size_t curr_depth = 1;


    auto moves = get_possible_moves(search_mem[indecies[curr_depth - 1]].board, player);
    std::cout << "Got " << moves.size() << " possible moves" << '\n';

    moves_size[curr_depth] = moves.size();
    if (moves_size[curr_depth] > BRANCH_FACTOR) {
        throw std::out_of_range("Max branch factor exceeded: " + std::to_string(moves_size[curr_depth]));
    }

    // Save all the next moves
    for (size_t i = 0; i < moves_size[curr_depth]; i++) {
        auto index = ((curr_depth - 1) *  BRANCH_FACTOR) + i;
        search_mem[index] = {
            board
        };
        search_mem[index].board.apply_move(moves[i]);
    }

    std::cout << "Done processing moves" << '\n';
    // If we're not at a leaf, evaluate at the next depth
    if (curr_depth < depth) {
        curr_depth++;
    }
    else {
        do {
            float best = 0.0;
            size_t best_index = 0;
            for (size_t i = 0; i < moves_size[curr_depth]; i++) {
                std::cout << "Evaluating leaf" << '\n';
                auto score = piece_count(board, player);
                if (score > best) {
                    best = score;
                    best_index = i;
                }
                auto index = ((curr_depth - 1) *  BRANCH_FACTOR) + i;
                search_mem[index].score = score;
            }
            std::cout << "best: " << best << '\n';
            std::cout << "best_index: " << best_index << '\n';
            curr_depth--;
            indecies[curr_depth]++;
            auto index = ((curr_depth - 1) *  BRANCH_FACTOR) + indecies[curr_depth - 1];
            std::cout << "search mem index: " << index << '\n';
            search_mem[index].score = best;
            search_mem[index].best_score_index = best_index;
        }
        while (curr_depth > 0 && indecies[curr_depth] == moves_size[curr_depth]);
    }

    auto best_index = BRANCH_FACTOR + search_mem[0].best_score_index;
    std::cout << "best_index: " << best_index << '\n';
    return std::make_pair(BoardState(search_mem[best_index].board), search_mem[0].score);
}
