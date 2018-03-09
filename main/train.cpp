// train.cpp
// Rohan Weeden
// Created: March 8, 2018

// Plays AI against eachother and evolves them.

#include "game.h"
#include "asknet4.cpp"
#include "search.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <initializer_list>

#define POOL_SIZE 50
#define TOPOLOGY (std::initializer_list<size_t>){32, 40, 10, 1}

void loadPoolFrom(const char * filename, std::vector<Network4> & pool);
void randomizePool(std::vector<Network4> & pool);


class NotImplementedException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "** NOT IMPLEMENTED **";
    }
};


class AIPlayer : public Player {
private:
    Network4 _net;

public:
    AIPlayer (int color, const Network4 & net):Player(color), _net(net) {}

    BoardState takeMove(const BoardState & board) override {
        // TODO: Replace with network search
        auto result = min_max_search_inplace(board, _color_id, 6);
        return result.first;
    }
};


int main(int argc, char const *argv[]) {
    std::vector<Network4> pool;
    pool.reserve(POOL_SIZE);

    if (argc > 1) {
        const char * filename = argv[1];
        std::cout << "Resuming training from file..." << '\n';
        loadPoolFrom(filename, pool);
    }
    else {
        randomizePool(pool);
    }

    size_t generation = 1;

    return 0;
}


void loadPoolFrom(const char * filename, std::vector<Network4> & pool) {
    throw NotImplementedException();
}


void randomizePool(std::vector<Network4> & pool) {
    for (size_t i = 0; i < POOL_SIZE; i++) {
        pool.emplace_back(TOPOLOGY);
        pool[pool.size() - 1].randomizeWeights();
    }
}
