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

    PieceCountPlayer p1(RED_PLAYER);
    Network4 beg({32, 40, 10, 1});
    Network4 mid({32, 40, 10, 1});
    Network4 end({32, 40, 10, 1});
    beg.readFromFile("beg_300_1.txt");
    mid.readFromFile("beg_300_1.txt");
    end.readFromFile("beg_300_1.txt");
    AIPlayer3Net p2(BLACK_PLAYER, beg, mid, end, 6);

    Game game(p1, p2);

    BoardState testBoard;
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        testBoard.set(i, BLANK);
    }

    testBoard.set(3, RED_CHECKER);
    testBoard.set(0, RED_CHECKER);
    testBoard.set(28, BLACK_CHECKER);
    testBoard.set(31, BLACK_CHECKER);

    // game.setBoard(testBoard);

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
            game.reset();
            game.randomizeOpeningMoves(3);
            // game.setBoard(testBoard);
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
