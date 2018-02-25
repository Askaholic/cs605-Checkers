// game_test.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Main file for testing the game play simulator

#include "game.h"
#include <iostream>

class UnPlayer : public Player {
private:
    /* data */

public:
    BoardState takeMove(const BoardState & board) const override{
        return board;
    }
};

int main(int argc, char const *argv[]) {
    UnPlayer p1;
    UnPlayer p2;

    Game game(p1, p2);

    game.playGame();
    std::cout << "Winner is: " << (game.getCurrentPlayer() == RED_PLAYER ? "red" : "black") << '\n';
    return 0;
}
