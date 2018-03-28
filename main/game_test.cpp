// game_test.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Main file for testing the game play simulator

#include "asknet4.h"
#include "game.h"
#include "board_funcs.h"
#include "jump_generator.h"
#include "search.h"
#include "piece_count.h"
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
        auto result = min_max_search_inplace(board, _color_id, 4, &piece_count);
        return result.first;
    }
};

class AIPlayer : public Player {
private:
    Network4 _net;

public:
    AIPlayer (int color, const Network4 & net):Player(color), _net(net) {}

    BoardState takeMove(const BoardState & board) override {
        auto result = min_max_search_inplace(board, _color_id, 4, std::bind(&AIPlayer::evaluate, this, std::placeholders::_1, std::placeholders::_2));
        return result.first;
    }

    float evaluate(const BoardState & board, int player) {
        std::vector<float> inputs(BOARD_ELEMENTS);
        for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
            auto piece = board[i];
            float score = 0;
            switch (piece) {
                case RED_CHECKER: score=1; break;
                case BLACK_CHECKER: score=-1; break;
                case RED_KING: score = _net.getKingValue(); break;
                case BLACK_KING: score = _net.getKingValue() * -1; break;
                default: score=0; break;
            }
            inputs[i] = score;
        }
        _net.setInputs(inputs);
        // _net.printWeights();
        auto result = _net.evaluate();
        if (player == BLACK_PLAYER) {
            result *= -1;
        }
        return result;
    }
};

int main(int argc, char const *argv[]) {
    bool pause_between_moves = false;
    if (argc > 1) {
        pause_between_moves = true;
    }

    RandomPlayer p1(RED_PLAYER);
    Network4 net({32, 40, 10, 1});
    net.readFromFile("best_network.txt");
    AIPlayer p2(BLACK_PLAYER, net);

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
