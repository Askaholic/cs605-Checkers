// game_test.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Main file for testing the game play simulator

#include "game.h"
#include "board_funcs.h"
#include "cstdlib"
#include <iostream>
#include <vector>

class UnPlayer : public Player {
private:
    /* data */

public:
    UnPlayer(int color):Player(color) {}

    BoardState takeMove(const BoardState & board) const override {
        return board;
    }
};

class RandomPlayer : public Player {
private:
    /* data */

public:
    RandomPlayer(int color):Player(color) {}

    BoardState takeMove(const BoardState & board) const override {
        auto moves = get_possible_moves(board, _color_id);
        BoardState next = board;

        if (moves.size() == 0) {
            return next;
        }

        next.apply_move(moves[std::rand() % moves.size()]);
        return next;
    }
};

int main(int argc, char const *argv[]) {
    RandomPlayer p1(RED_PLAYER);
    RandomPlayer p2(BLACK_PLAYER);

    Game game(p1, p2);

    std::cout << "Starting game..." << '\n';
    game.playGame();
    std::cout << "Winner is: " << (game.getCurrentPlayer() == RED_PLAYER ? "red" : "black") << '\n';
    print_board(game.getBoard());
    return 0;
}
