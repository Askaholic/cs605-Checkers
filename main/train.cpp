// train.cpp
// Rohan Weeden
// Created: March 8, 2018

// Plays AI against eachother and evolves them.

#include "game.h"
#include "asknet4.cpp"
#include "search.h"

class AIPlayer : Player {
private:
    Network4 _net;

public:
    AIPlayer (int color):Player(color), _net({32, 40, 10, 1}) {}

    BoardState takeMove(const BoardState & board) override {
        // TODO: Replace with network search
        auto result = min_max_search_inplace(board, _color_id, 6);
        return result.first;
    }
};

int main(int argc, char const *argv[]) {

    return 0;
}
