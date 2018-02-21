// little_search.cpp
// Rohan Weeden
// Created: Feb. 19, 2018

// Minimax search using in place alocation

#include "board_funcs.h"

#define BRANCH_FACTOR 18

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
        curr_depth = 0;
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
                default: next='.'; break;
            }
            std::cout << next;
        }
        std::cout << '\n';
    }

    void expandChildren(size_t * indecies, size_t * moves_size, int player) {
        auto board = search_mem[ (curr_depth) * branch_factor + indecies[curr_depth] ].board;
        auto jumps = get_possible_jumps(board, player);
        if (jumps.size() > 0) {
            std::cout << "Found jumps" << '\n';
            moves_size[curr_depth + 1] = jumps.size();
            // Save all the next jumps
            for (size_t i = 0; i < moves_size[curr_depth + 1]; i++) {
                auto index = ((curr_depth + 1) *  BRANCH_FACTOR) + i;
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
            std::cout << "current_depth " << curr_depth << '\n';
            std::cout << "indecies[curr_depth] " << indecies[curr_depth] << '\n';
            printBoard(board);
        }
        moves_size[curr_depth + 1] = moves.size();
        if (moves_size[curr_depth + 1] > BRANCH_FACTOR) {
            std::cout << "Caused by board: " << indecies[curr_depth] << '\n';
            printBoard(board);
            std::cout << "Tree nodes: " << tree_nodes << '\n';
            throw std::out_of_range("Max branch factor exceeded: " + std::to_string(moves_size[curr_depth]));
        }

        // Save all the next moves
        for (size_t i = 0; i < moves_size[curr_depth + 1]; i++) {
            auto index = ((curr_depth + 1) *  BRANCH_FACTOR) + i;
            search_mem[index] = {};
            search_mem[index].board = board;
            search_mem[index].score = 0.0f;
            search_mem[index].best_score_index = 0;

            search_mem[index].board.apply_move(moves[i]);
            // printBoard(search_mem[index].board);
        }
    }

    float evaluateLeaf(const BoardState & board, int player) {
        if(curr_depth & 0x1) {
            // Odd depth
            return piece_count(board, player);
        }
        // Even depth, that means maximize for the opposite player
        return piece_count(board, player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER);
    }

    float evaluateLeaves(size_t * moves_size, const BoardState & board, int player) {
        for (size_t i = 0; i < moves_size[curr_depth]; i++) {
            tree_nodes++;
            // std::cout << "Evaluating leaf: ";
            auto score = evaluateLeaf(board, player);
            // std::cout << score << '\n';
            auto index = ((curr_depth) *  branch_factor) + i;
            search_mem[index].score = score;
        }
    }

    void nextIndex(size_t * indecies, int depth) {

    }

    std::pair<BoardState, float> search(const BoardState & board, int player, int depth) {
        initData(depth);

        size_t indecies[depth] = {0};
        size_t moves_size[depth] = {0};
        search_mem[0].board = board;

        tree_nodes = 0;
        bool done = false;
        while (!done) {
            if (curr_depth == depth) {
                evaluateLeaves(moves_size, board, player);
            }
            else if (indecies[curr_depth] == 0) {
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
                    // Find the max value of this level
                    float best = -1000000;
                    size_t best_index = 0;

                    for (size_t i = 0; i < moves_size[curr_depth]; i++) {
                        auto index = ((curr_depth) * branch_factor) + i;
                        auto score = search_mem[index].score;
                        if (score > best) {
                            best = score;
                            best_index = i;
                        }
                    }

                    // Set this best score to the score of the parent node
                    curr_depth--;

                    auto index = ((curr_depth) *  branch_factor) + indecies[curr_depth];

                    // std::cout << "Setting index: "<< index << " to " << best << '\n';
                    search_mem[index].score = best;
                    search_mem[index].best_score_index = best_index;

                    indecies[curr_depth]++;
                    if (curr_depth == 0) {
                        // We've hit the root node, so we're done
                        done = true;
                    }
                }
                while (curr_depth > 0 && indecies[curr_depth] == moves_size[curr_depth]);
            }
        }

        auto best_index = BRANCH_FACTOR + search_mem[0].best_score_index;
        std::cout << "best_index: " << best_index << '\n';
        std::cout << "Tree nodes: " << tree_nodes << '\n';
        printBoard(search_mem[best_index].board);
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
