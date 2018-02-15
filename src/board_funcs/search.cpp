// search.cpp
// Rohan Weeden
// Created: Feb. 9, 2018

// Implementation for searching functions

#include "board_funcs.h"
#include "math.h"
#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

void min_max_search_helper(std::pair<std::unique_ptr<BoardState>, int> &, const BoardState & board, int player, int depth);
void min_max_search_helper_ab(std::pair<BoardState, int> &, const BoardState & board, int player, int depth, int alpha, int beta);
std::pair<size_t, int> min_max_no_alloc_helper(int player, size_t depth, char * mem, size_t mem_size, size_t mem_offset, size_t branch_factor);

std::pair<std::unique_ptr<BoardState>, int> min_max_search(const BoardState & board, int player, int depth) {
    auto result = std::make_pair<std::unique_ptr<BoardState>, int>(nullptr, 0);
    min_max_search_helper(result, board, player, depth);
    return result;
}

size_t leaves = 0;
std::pair<BoardState, int> min_max_search_ab(const BoardState & board, int player, int depth) {
    auto result = std::make_pair<BoardState, int>(BoardState(), 0);
    min_max_search_helper_ab(result, board, player, depth, 0, 0);
    std::cout << "leaves: " << leaves << '\n';
    return result;
}

std::pair<BoardState, int> min_max_no_alloc(const BoardState & board, int player, int depth) {
    auto result = std::make_pair<BoardState, int>(BoardState(board), 0);
    size_t branch_factor = 11;

    size_t nodes = (pow(branch_factor, depth + 1) - 1) / (branch_factor - 1);
    size_t mem_size = sizeof(BoardState) * nodes;
    std::cout << "Allocating " << mem_size << " bytes for tree wih " << nodes << " nodes" << '\n';
    char * mem = new char[mem_size];

    try {
        // Write the initial board into the root node
        board_write(mem, board);
        auto search_result = min_max_no_alloc_helper(player, depth, mem, mem_size, 0, branch_factor);
        // std::cout << "Got offset: " << search_result.first << '\n';
        board_read(&mem[search_result.first], result.first);
        result.second = search_result.second;
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
    if (depth <= 1) {
        result.first = std::make_unique<BoardState>(
            BoardState(board)
        );
        result.second = piece_count(board, player);
        return;
    }

    // Generate all possible next board states
    std::vector<BoardState> next_boards;
    auto jumps = get_possible_jumps(board, player);

    if (jumps.size() != 0) {
        for (size_t i = 0; i < jumps.size(); i++) {
            auto next = BoardState(board);
            next.apply_jump(jumps[i]);
            next_boards.push_back(next);
        }
    }
    else {
        auto moves = get_possible_moves(board, player);
        for (size_t i = 0; i < moves.size(); i++) {
            auto next = BoardState(board);
            next.apply_move(moves[i]);
            next_boards.push_back(next);
        }
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
            best_index = i;
            best = next_result.second;
        }
    }
    result.first = std::make_unique<BoardState>(next_boards[best_index]);
    result.second = best;

    return;
}

void verify_board_state(const BoardState & board) {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        bool good = false;
        switch (board[i]) {
            case BLANK: good=true; break;
            case BLACK_CHECKER: good=true; break;
            case BLACK_KING: good=true; break;
            case RED_CHECKER: good=true; break;
            case RED_KING: good=true; break;
        }
        if (!good) {
            std::cout << "bad piece detected" << '\n';
        }
    }
}
// DFS min max search
void min_max_search_helper_ab(std::pair<BoardState, int> & result, const BoardState & board, int player, int depth, int alpha, int beta) {
    verify_board_state(board);
    if (depth <= 1) {
        leaves++;
        // std::cout << "leaves: " << leaves << '\n';
        result.first = BoardState(board);
        result.second = piece_count(board, player);
        return;
    }

    // Generate all possible next board states
    std::vector<BoardState> next_boards;
    auto jumps = get_possible_jumps(board, player);

    if (jumps.size() != 0) {
        for (size_t i = 0; i < jumps.size(); i++) {
            auto next = BoardState(board);
            next.apply_jump(jumps[i]);
            next_boards.push_back(next);
        }
    }
    else {
        auto moves = get_possible_moves(board, player);
        for (size_t i = 0; i < moves.size(); i++) {
            auto next = BoardState(board);
            next.apply_move(moves[i]);
            next_boards.push_back(next);
        }
    }

    int best = 0;
    size_t best_index = 0;
    std::pair<BoardState, int> next_result;

    if (next_boards.size() == 0) {
        result.first = BoardState(board);
        result.second = 0;
        return;
    }
    for (size_t i = 0; i < next_boards.size(); i++) {
        min_max_search_helper_ab(
            next_result,
            next_boards[i],
            player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER,
            depth - 1,
            alpha,
            beta
        );
        if (i == 0 || next_result.second > best) {
            best_index = i;
            best = next_result.second;
        }
        if (player == RED_PLAYER && best > alpha) {
            alpha = best;
        }
        else if (player == BLACK_PLAYER && best > beta) {
            beta = best;
        }
        if (beta <= alpha){
            break;
        }
    }
    result.first = BoardState(next_boards[best_index]);
    result.second = best;

    return;
}

size_t get_child_offset(size_t parent, size_t branch_factor, size_t i) {
    return (branch_factor * parent / sizeof(BoardState)  + i + 1) * sizeof(BoardState);
}

std::pair<size_t, int> min_max_no_alloc_helper(int player, size_t depth, char * mem, size_t mem_size, size_t mem_offset, size_t branch_factor) {
    if (depth <= 1) {
        auto score = piece_count(&mem[mem_offset], player);
        // Slow?
        return std::make_pair(mem_offset, score);
    }
    // Generate all possible next board states
    auto moves = get_possible_moves(&mem[mem_offset], player);

    if (branch_factor < moves.size()) {
        throw std::out_of_range("Move size " + std::to_string(moves.size()) + " exceeds branch factor " + std::to_string(branch_factor));
    }

    int best = 0;
    size_t best_offset = 0;

    for (size_t i = 0; i < moves.size(); i++) {
        auto child_offset = get_child_offset(mem_offset, branch_factor, i);
        if (child_offset > mem_size) {
            std::cout << "Uh oh... I'm node: " << mem_offset / sizeof(BoardState) << '\n';
            std::cout << "child_offset: " << child_offset << '\n';
        }
        board_copy(&mem[mem_offset], &mem[child_offset]);
        board_apply_move(&mem[child_offset], moves[i]);

        auto next_result = min_max_no_alloc_helper(
            player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER,
            depth - 1,
            mem,
            mem_size,
            child_offset,
            branch_factor
        );

        if (i == 0 || next_result.second > best) {
            best_offset = child_offset;
            best = next_result.second;
        }
    }

    return std::make_pair(best_offset, best);
}
