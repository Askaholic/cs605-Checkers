// little_search.cpp
// Rohan Weeden
// Created: Feb. 19, 2018

// Minimax search using in place alocation

#include "board_funcs.h"

#define BRANCH_FACTOR 16

struct SearchNode {
    BoardState board;
    float score;
    size_t best_score_index;
};

class MinMaxSearch {
private:
    std::vector<SearchNode> search_mem;
    size_t branch_factor = BRANCH_FACTOR;
    size_t tree_nodes = 0;
    size_t curr_depth;

public:
    void initData(int depth) {
        search_mem = std::vector<SearchNode>((depth + 1) * branch_factor);
        curr_depth = 1;
    }

    void printBoard(const BoardState & board) {
        for (size_t i = 0; i < 32; i++) {
            auto piece = board[i];
            char next = '.';
            switch (piece) {
                case RED_CHECKER: next='r'; break;
                case BLACK_CHECKER: next='b'; break;
                case RED_KING: next='R'; break;
                case BLACK_KING: next='B'; break;
            }
            std::cout << next;
        }
        std::cout << '\n';
    }

    void expandChildren(size_t * indecies, size_t * moves_size, int player) {
        auto board = search_mem[(curr_depth - 1) * branch_factor + indecies[curr_depth - 1]].board;
        auto jumps = get_possible_jumps(board, player);
        if (jumps.size() != 0) {
            std::cout << "got some jumps" << '\n';
            moves_size[curr_depth] = jumps.size();
            // Save all the next jumps
            for (size_t i = 0; i < moves_size[curr_depth]; i++) {
                auto index = ((curr_depth) *  BRANCH_FACTOR) + i;
                search_mem[index] = {};
                search_mem[index].board = board;
                search_mem[index].score = 0.0f;
                search_mem[index].best_score_index = 0;

                search_mem[index].board.apply_jump(jumps[i]);
                // printBoard(search_mem[index].board);
            }
            return;
        }
        auto moves = get_possible_moves(board, player);
        // std::cout << "Got " << moves.size() << " possible moves at depth " <<curr_depth << '\n';

        if (moves.size() == 0) {
            std::cout << "No move board found!!!" << '\n';
            std::cout << "current_depth - 1 " << curr_depth - 1 << '\n';
            std::cout << "indecies[curr_depth - 1] " << indecies[curr_depth - 1] << '\n';
            printBoard(board);
        }
        moves_size[curr_depth] = moves.size();
        if (moves_size[curr_depth] > BRANCH_FACTOR) {
            std::cout << "Caused by board: " << indecies[curr_depth - 1] << '\n';
            printBoard(board);
            std::cout << "Tree nodes: " << tree_nodes << '\n';
            throw std::out_of_range("Max branch factor exceeded: " + std::to_string(moves_size[curr_depth]));
        }

        // Save all the next moves
        for (size_t i = 0; i < moves_size[curr_depth]; i++) {
            auto index = ((curr_depth) *  BRANCH_FACTOR) + i;
            search_mem[index] = {};
            search_mem[index].board = board;
            search_mem[index].score = 0.0f;
            search_mem[index].best_score_index = 0;

            search_mem[index].board.apply_move(moves[i]);
            // printBoard(search_mem[index].board);
        }
    }

    float evaluateLeaf(const BoardState & board, int player) {
        return piece_count(board, player);
    }

    float evaluateLeaves(size_t * moves_size, float & best, size_t & best_index, const BoardState & board, int player) {
        for (size_t i = 0; i < moves_size[curr_depth]; i++) {
            tree_nodes++;
            // std::cout << "Evaluating leaf" << '\n';
            auto score = evaluateLeaf(board, player);
            if (score > best) {
                best = score;
                best_index = i;
            }
            auto index = ((curr_depth - 1) *  branch_factor) + i;
            search_mem[index].score = score;
        }
    }

    std::pair<BoardState, float> search(const BoardState & board, int player, int depth) {
        initData(depth);

        size_t indecies[depth] = {0};
        size_t moves_size[depth] = {0};
        search_mem[0].board = board;

        tree_nodes = 0;
        while (curr_depth > 0) {
            if (indecies[curr_depth] == 0) {
                expandChildren(indecies, moves_size, player);
            }

            // std::cout << "Done processing moves" << '\n';
            // If we're not at a leaf, evaluate at the next depth
            if (curr_depth < depth) {
                // std::cout << "inc depth" << '\n';
                curr_depth++;
                indecies[curr_depth] = 0;
            }
            else {
                do {
                    float best = 0.0;
                    size_t best_index = 0;
                    evaluateLeaves(moves_size, best, best_index, board, player);

                    curr_depth--;
                    indecies[curr_depth]++;

                    auto index = ((curr_depth) *  branch_factor) + indecies[curr_depth];

                    search_mem[index].score = best;
                    search_mem[index].best_score_index = best_index;
                }
                while (curr_depth > 0 && indecies[curr_depth] == moves_size[curr_depth]);
            }
        }

        auto best_index = BRANCH_FACTOR + search_mem[0].best_score_index;
        std::cout << "best_index: " << best_index << '\n';
        std::cout << "Tree nodes: " << tree_nodes << '\n';
        return std::make_pair(BoardState(search_mem[best_index].board), search_mem[0].score);
    }
};

std::pair<BoardState, float> min_max_search_inplace(const BoardState & board, int player, int depth) {
    if (depth < 1) {
        return std::make_pair<BoardState, float>(
            BoardState(board),
            (float) piece_count(board, player)
        );
    }

    MinMaxSearch s_helper;
    return s_helper.search(board, player, depth);
}
