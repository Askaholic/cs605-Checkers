// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include "asknet.h"
#include "asknet4.h"
#include "board_funcs.h"
#include "game.h"
#include "jump_table.h"
#include "move_table.h"
#include "player.h"
#include <vector>
#include <cstddef>
#include <iostream>

Network4 the_net_beg({0, 0});
Network4 the_net_mid({0, 0});
Network4 the_net_end({0, 0});
AIPlayer3Net the_player(RED_PLAYER, the_net_beg, the_net_mid, the_net_end);

void setup_network(int playerColor) {
    the_net_beg = Network4({32, 1000, 40, 10, 1});
    the_net_mid = Network4({32, 1000, 40, 10, 1});
    the_net_end = Network4({32, 1000, 40, 10, 1});

    // the_net_beg.readFromFile("net_beg_best.txt");
    the_net_beg.randomizeWeights();
    std::cout << "Initialized network with " << the_net_beg.getNumNodes() << " nodes" << '\n';

    // the_net_beg.readFromFile("net_mid_best.txt");
    the_net_beg.randomizeWeights();
    std::cout << "Initialized network with " << the_net_mid.getNumNodes() << " nodes" << '\n';

    // the_net_beg.readFromFile("net_end_best.txt");
    the_net_beg.randomizeWeights();
    std::cout << "Initialized network with " << the_net_end.getNumNodes() << " nodes" << '\n';

    the_player = AIPlayer3Net(playerColor, the_net_beg, the_net_mid, the_net_end);
}

BoardState make_move(const BoardState & board) {
    return the_player.takeMove(board);
}


void _set_targets(char * targets, int player) {
    if (player == RED_PLAYER) {
        targets[0] = RED_CHECKER;
        targets[1] = RED_KING;
    }
    else {
        targets[0] = BLACK_CHECKER;
        targets[1] = BLACK_KING;
    }
}


std::vector<Move> get_possible_moves(const BoardState &board, int player) {
    std::vector<Move> moves;

    char targets[2];
    _set_targets(targets, player);

    for (size_t b_loc = 0; b_loc < BOARD_ELEMENTS; b_loc++) {

        auto current_piece = board[b_loc];
        // Skip squares that don't have the correct color checker
        if (!in_<char>(targets, 2, current_piece)) {
            continue;
        }

        auto possible_moves = moveTable[b_loc];
        size_t start = 0;
        size_t end = possible_moves.size();

        // Grab only the north direction
        if (current_piece == BLACK_CHECKER) {
            end -= 2;
        }
        // grab only the south direction
        else if (current_piece == RED_CHECKER) {
            start += 2;
        }

        for (size_t c = start; c < end; c++) {
            auto move = possible_moves[c];

            if (move < 0) { continue; }

            if (board[move] == BLANK) {
                moves.push_back({b_loc, (size_t) move});
            }
        }
    }

    return moves;
}


bool is_valid_jump(const BoardState & board, const Jump & jump, int player) {

    if (board[jump._to] != BLANK){
       return false;
    }

    char targets[2];
    _set_targets(targets, player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER);

    if (!in_<char>(targets, 2, board[jump._enemy]) ){
        return false;
    }

    return true;
}


BoardState make_board(const std::vector<char> & board) {
    if (board.size() != BOARD_ELEMENTS) {
        throw std::out_of_range("Wrong number of board elements");
    }
    BoardState board_state;

    for (size_t i = 0; i < board.size(); i++) {
        char value = BLANK;
        switch (board[i]) {
            case 'r': value = RED_CHECKER; break;
            case 'b': value = BLACK_CHECKER; break;
            case 'R': value = RED_KING; break;
            case 'B': value = BLACK_KING; break;
        }
        board_state.set(i, value);
    }
    return board_state;
}
