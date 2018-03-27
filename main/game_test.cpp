// game_test.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Main file for testing the game play simulator

#include "game.h"
#include "board_funcs.h"
#include "jump_generator.h"
#include "search.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

class UnPlayer : public Player {
private:
    /* data */

public:
    UnPlayer(int color):Player(color) {}

    BoardState takeMove(const BoardState & board) override {
        return board;
    }
};

class RandomPlayer : public Player {
private:
    JumpGenerator gen;

public:
    RandomPlayer(int color):Player(color) {
        std::srand(std::time(nullptr));
    }

    BoardState takeMove(const BoardState & board) override {
        auto jumps = gen.get_possible_jumps(board, _color_id);
        if (jumps.size() != 0) {
            return jumps[std::rand() % jumps.size()];
        }

        auto moves = get_possible_moves(board, _color_id);
        BoardState next = board;

        if (moves.size() == 0) {
            return next;
        }

        next.apply_move(moves[std::rand() % moves.size()]);
        return next;
    }
};

class PieceCountPlayer : public Player{
private:
    /* data */

public:
    PieceCountPlayer (int color):Player(color) {}

    BoardState takeMove(const BoardState & board) override {
        auto result = min_max_search_inplace(board, _color_id, 4);
        return result.first;
    }
};

int main(int argc, char const *argv[]) {
    bool pause_between_moves = false;
    if (argc > 1) {
        pause_between_moves = true;
    }

    RandomPlayer p1(RED_PLAYER);
    PieceCountPlayer p2(BLACK_PLAYER);

    Game game(p1, p2);

    std::cout << "Starting game..." << '\n';
    if (pause_between_moves) {
        char s[1];
        while (!game.hasWinner()) {
            game.playTurn();
            print_board(game.getBoard());

            std::cout << "/* press to continue... */" << '\n';
            std::cin.getline(s, 1);
        }
    }
    else {
        game.playGame();
    }
    auto winner = game.getWinner();
    if (winner == -1) {
        std::cout << "Game was a draw" << '\n';
    }
    else {
        std::cout << "Winner is: " << (winner == RED_PLAYER ? "red" : "black") << '\n';
    }
    print_board(game.getBoard());
    return 0;
}