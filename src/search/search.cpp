// little_search.cpp
// Rohan Weeden
// Created: Feb. 19, 2018

// Minimax search using in place alocation

#include "search.h"
#include "jump_generator.h"
#include "piece_count.h"
#include "board_funcs.h"
#include <cstddef>
#include <exception>
#include <iostream>
#include <vector>

struct SearchNode {
    BoardState board;
    float score;
    size_t best_score_index;
};

class MinMaxSearch {
private:
    std::vector<SearchNode> search_mem;
    size_t branch_factor = BRANCH_FACTOR;
    size_t max_branch_factor = 0;
    size_t tree_nodes = 0;
    size_t curr_depth;
    int maximizing_player;
    JumpGenerator jump_gen;

public:
    void initData(int depth) {
        search_mem = std::vector<SearchNode>((depth + 1) * branch_factor);
        curr_depth = 0;
    }

    int getCurrentTurnPlayer(int player) {
        if(curr_depth & 0x1) {
            // Odd depth
            return player;
        }
        // Even depth, that means maximize for the opposite player
        return swapPlayer(player);
    }

    int swapPlayer(int player) {
        return player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER;
    }

    void expandChildren(size_t * indecies, size_t * moves_size, int player) {
        auto board = search_mem[ (curr_depth) * branch_factor + indecies[curr_depth] ].board;
        player = swapPlayer(getCurrentTurnPlayer(player));

        std::vector<BoardState> jumps = jump_gen.get_possible_jumps(board, player);

        if (jumps.size() > 0) {
            moves_size[curr_depth + 1] = jumps.size();
            if (jumps.size() > max_branch_factor) {
                max_branch_factor = jumps.size();
            }
            // Save all the next jumps
            for (size_t i = 0; i < moves_size[curr_depth + 1]; i++) {
                auto index = ((curr_depth + 1) *  BRANCH_FACTOR) + i;
                search_mem[index] = {};
                search_mem[index].board = jumps[i];
                search_mem[index].score = 0.0f;
                search_mem[index].best_score_index = 0;
            }
            return;
        }
        auto moves = jump_gen.get_possible_moves(board, player);

        moves_size[curr_depth + 1] = moves.size();
        if (moves.size() > max_branch_factor) {
            max_branch_factor = jumps.size();
        }
        if (moves_size[curr_depth + 1] > BRANCH_FACTOR) {
            std::cout << "Caused by board: " << indecies[curr_depth] << '\n';
            print_board(board);
            std::cout << "Tree nodes: " << tree_nodes << '\n';
            throw std::out_of_range("Max branch factor exceeded: " + std::to_string(moves_size[curr_depth + 1]));
        }

        // Save all the next moves
        for (size_t i = 0; i < moves_size[curr_depth + 1]; i++) {
            auto index = ((curr_depth + 1) *  BRANCH_FACTOR) + i;
            search_mem[index] = {};
            search_mem[index].board = moves[i];
            search_mem[index].score = 0.0f;
            search_mem[index].best_score_index = 0;

            // print_board(search_mem[index].board);
            // std::cout << '\n';
        }
    }

    void evaluateLeaves(size_t * moves_size, std::function<float(const BoardState &, int)> evaluate) {
        for (size_t i = 0; i < moves_size[curr_depth]; i++) {
            tree_nodes++;
            auto index = ((curr_depth) *  branch_factor) + i;
            auto score = evaluate(search_mem[index].board, maximizing_player);
            search_mem[index].score = score;
        }
    }

    std::pair<BoardState, float> search(const BoardState & board, int player, int depth, std::function<float(const BoardState &, int)> evaluate) {
        initData(depth);
        maximizing_player = player;

        size_t indecies[depth] = {0};
        size_t moves_size[depth] = {0};
        search_mem[0].board = board;

        tree_nodes = 0;
        bool done = false;
        while (!done) {
            if (indecies[curr_depth] > moves_size[curr_depth]) {
                throw std::out_of_range("Index at current depth exceeded number of moves! " + std::to_string(indecies[curr_depth]));
            }
            if (curr_depth == (unsigned int) depth) {
                evaluateLeaves(moves_size, evaluate);
            }
            else {
                expandChildren(indecies, moves_size, player);
            }

            // If we're not at a leaf, evaluate at the next depth
            if (curr_depth < (unsigned int) depth) {
                curr_depth++;
                indecies[curr_depth] = 0;
            }
            else {
                do {
                    // Find the max value of this level
                    float best;
                    if (player == getCurrentTurnPlayer(player)) {
                        best = -100;
                    }
                    else {
                        best = 100;
                    }
                    size_t best_index = 0;

                    for (size_t i = 0; i < moves_size[curr_depth]; i++) {
                        auto index = ((curr_depth) * branch_factor) + i;

                        if(i == 0) {
                            best = search_mem[index].score;
                        }
                        auto score = search_mem[index].score;
                        if (player == getCurrentTurnPlayer(player)) {
                            // Max node
                            if (score > best) {
                                best = score;
                                best_index = i;
                            }
                        }
                        else {
                            // Min node
                            if (score < best) {
                                best = score;
                                best_index = i;
                            }
                        }
                        // if (curr_depth == 1 || curr_depth == 2) {
                        //     if (curr_depth == 2) {
                        //         std::cout << "\t";
                        //     }
                        //     std::cout << "Score at " << i << ": " << score << '\n';
                        // }
                    }

                    curr_depth--;

                    auto index = ((curr_depth) *  branch_factor) + indecies[curr_depth];

                    // Set this best score to the score of the parent node
                    search_mem[index].score = best;
                    search_mem[index].best_score_index = best_index;

                    indecies[curr_depth]++;
                    if (curr_depth == 0) {
                        // We've hit the root node, so we're done
                        done = true;
                    }
                }
                while (curr_depth > 0 && indecies[curr_depth] >= moves_size[curr_depth]);
            }
        }

        auto best_index = BRANCH_FACTOR + search_mem[0].best_score_index;
        return std::make_pair(BoardState(search_mem[best_index].board), search_mem[0].score);
    }
};

class MinMaxSearchRecurse {
private:
    JumpGenerator jump_gen;

public:
    bool usePrune = false;

    std::vector<BoardState> get_children(const BoardState & board, int player) {
        std::vector<BoardState> jumps = jump_gen.get_possible_jumps(board, player);

        if (jumps.size() > 0) {
            return jumps;
        }
        return jump_gen.get_possible_moves(board, player);
    }


    std::pair<BoardState, float> search(const BoardState & board, int maximizing_player, int player, int depth, int max_depth, float alpha, float beta, std::function<float(const BoardState &, int)> evaluate) {

        if (depth == max_depth + 1) {
            return std::make_pair<BoardState, float>(
                BoardState(board),
                evaluate(board, maximizing_player)
            );
        }

        if (player == maximizing_player) {
            float best = -100;
            int best_index = -1;
            auto children = get_children(board, player);
            // std::cout << "num children: " << children.size() << '\n';
            for (size_t i = 0; i < children.size(); i++) {
                auto result = search(children[i], maximizing_player, swapPlayer(player), depth + 1, max_depth, alpha, beta, evaluate);

                // std::string prefix = "";
                // for (size_t j = 0; j < depth; j++) {
                //     prefix += "\t";
                // }
                // print_board(children[i], prefix);
                // std::cout << prefix << "score: " << result.second << '\n';
                // std::cout << '\n';

                if (result.second > best) {
                    best = result.second;
                    best_index = i;
                }
                if (best > alpha) {
                    alpha = best;
                }
                if (usePrune && beta <= alpha) {
                    break;
                }
            }
            return std::make_pair<BoardState, float>(
                (BoardState)(best_index == -1 ? board : children[best_index]),
                float(best)
            );
        }
        else {
            float best = 100;
            int best_index = -1;
            auto children = get_children(board, player);

            for (size_t i = 0; i < children.size(); i++) {
                auto result = search(children[i], maximizing_player, swapPlayer(player), depth + 1, max_depth, alpha, beta, evaluate);

                // std::string prefix = "";
                // for (size_t j = 0; j < depth; j++) {
                //     prefix += "\t";
                // }
                // print_board(children[i], prefix);
                // std::cout << prefix << "score: " << result.second << '\n';
                // std::cout << '\n';

                if (result.second < best) {
                    best = result.second;
                    best_index = i;
                }
                if (best < beta) {
                    beta = best;
                }
                if (usePrune && beta <= alpha) {
                    break;
                }
            }
            return std::make_pair<BoardState, float>(
                (BoardState)(best_index == -1 ? board : children[best_index]),
                float(best)
            );
        }
    }

    int swapPlayer(int player) {
        return player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER;
    }

};


std::pair<BoardState, float> min_max_search(const BoardState & board, int player, int depth, std::function<float(const BoardState &, int)> evaluate) {
    if (depth < 1) {
        return std::make_pair<BoardState, float>(
            BoardState(board),
            evaluate(board, player)
        );
    }

    MinMaxSearchRecurse helper;
    helper.usePrune = true;
    auto result = helper.search(board, player, player, 1, depth, -100, 100, evaluate);
    return result;
}


std::pair<BoardState, float> min_max_search_inplace(const BoardState & board, int player, int depth, std::function<float(const BoardState &, int)> evaluate) {
    if (depth < 1) {
        return std::make_pair<BoardState, float>(
            BoardState(board),
            evaluate(board, player)
        );
    }

    MinMaxSearch s_helper;
    return s_helper.search(board, player, depth, evaluate);
}
