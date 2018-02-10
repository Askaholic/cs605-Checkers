// search.cpp
// Rohan Weeden
// Created: Feb. 9, 2018

// Implementation for searching functions

#include "board_funcs.h"
#include "math.h"
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

void min_max_search_helper(std::pair<std::unique_ptr<BoardState>, int> &, const BoardState & board, int player, int depth);
void min_max_no_alloc_helper(std::pair<BoardState, int> &, int player, size_t depth, char * mem, size_t mem_length, size_t branch_factor);

std::pair<std::unique_ptr<BoardState>, int> min_max_search(const BoardState & board, int player, int depth) {
    auto result = std::make_pair<std::unique_ptr<BoardState>, int>(nullptr, 0);
    min_max_search_helper(result, board, player, depth);
    return result;
}

std::pair<BoardState, int> min_max_no_alloc(const BoardState & board, int player, int depth) {
    auto result = std::make_pair<BoardState, int>(BoardState(board), 0);
    size_t branch_factor = 10;
    size_t mem_size = sizeof(BoardState) * (branch_factor + depth);
    std::cout << "Allocating " << mem_size << " bytes for tree" << '\n';
    char * mem = new char[mem_size];

    try {
        // Write the initial board into the root node
        board_write(mem, board);
        min_max_no_alloc_helper(result, player, depth, mem, mem_size, branch_factor);
    }
    catch (...) {
        // Yea I'm a bit paranoid
        free(mem);
        throw;
    }
    free(mem);
    return result;
}

// DFS min max search
void min_max_search_helper(std::pair<std::unique_ptr<BoardState>, int> & result, const BoardState & board, int player, int depth) {
    if (depth < 1) {
        result.first = std::make_unique<BoardState>(
            BoardState(board)
        );
        result.second = piece_count(board, player);
        return;
    }

    // Generate all possible next board states
    auto moves = get_possible_moves(board, player);
    std::vector<BoardState> next_boards;
    for (size_t i = 0; i < moves.size(); i++) {
        auto next = BoardState(board);
        next.apply_move(moves[i]);
        next_boards.push_back(next);
    }

    int best = 0;
    size_t best_index = 0;
    std::pair<std::unique_ptr<BoardState>, int> next_result;

    for (size_t i = 0; i < next_boards.size(); i++) {
        min_max_search_helper(
            next_result,
            next_boards[i],
            player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER,
            depth - 1
        );
        if (i == 0 || next_result.second > best) {
            // TODO: Is this an extra copy?
            best_index = i;
            best = next_result.second;
        }
    }
    result.first = std::make_unique<BoardState>(next_boards[best_index]);
    result.second = best;

    return;
}

int get_index(int depth) {

}

void min_max_no_alloc_helper(std::pair<BoardState, int> & result, int player, size_t depth, char * mem, size_t mem_size, size_t branch_factor) {
    // Index at each search depth

    int best = 0;

    std::vector<bool> visited((size_t)pow(branch_factor, depth), false);
    std::stack<size_t> stack;
    while (!stack.empty()) {
        auto i = stack.top();
        stack.pop();

        if (visited[i]) {
            std::cout << "Visited " << i << " alerady..." << '\n';
            continue;
        }

        std::cout << "Visited: " << i << '\n';
        visited[i] = true;

        for (size_t i = 0; i < branch_factor; i++) {
            /* code */
        }
        // if (indecies[current_depth] >= depth) {
        //     // tree is done
        //     best = piece_count(mem, player);
        //     board_read(mem, result.first);
        //     result.second = best;
        //     break;
        // }


        // moves[current_depth] = get_possible_moves(mem, player);
        // if (current_depth == depth - 1) {
        //
        // }
        break;
    }
}
