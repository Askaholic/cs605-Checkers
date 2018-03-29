// game_test.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Main file for testing the game play simulator

#include "game.h"
#include "board_funcs.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>


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
        while (!game.hasWinner()) {
            std::cout << "Player: " << (game.getCurrentPlayer() == RED_PLAYER ? "Red" : "Black") << '\n';
            print_board(game.getBoard());
            game.playTurn();

            std::cout << "/* press to continue... */" << '\n';
            std::cin.get();
        }
    }
    else {
        float scores[2] = {0,0};
        for (size_t i = 0; i < 100; i++) {
            game.playGame();
            auto winner = game.getWinner();
            if (winner == RED_PLAYER) {
                scores[0]++;
            }
            else if (winner == BLACK_PLAYER) {
                scores[1]++;
            }
        }
        std::cout << "Red won: " << scores[0] << " times" << '\n';
        std::cout << "Black won: " << scores[1] << " times" << '\n';
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
