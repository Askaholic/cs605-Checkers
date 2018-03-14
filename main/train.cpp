// train.cpp
// Rohan Weeden
// Created: March 8, 2018

// Plays AI against eachother and evolves them.

#include "game.h"
#include "asknet4.cpp"
#include "search.h"
#include "board_funcs.h"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#define STARTING_POOL_SIZE 10
/* Must be less than STARTING_POOL_SIZE.
 * Describes the number of networks that survive to the next generation.
 */
#define SURVIVAL_CUTTOFF 5
#define NUM_OFFSPRING 2
#define GENERATION_TARGET 300
#define NUM_GAMES 5
#define WIN_POINTS 1
#define LOSS_POINTS -1
#define DRAW_POINTS 0
#define TOPOLOGY (std::initializer_list<size_t>){32, 40, 10, 1}


class ScoredNetwork {
public:
    Network4 net;
    int score;
    size_t games_played;

public:
    ScoredNetwork(std::vector<size_t> topology, int score, size_t games_played)
        :net(topology), score(score), games_played(games_played) { }

    ScoredNetwork(const Network4 & net, int score, size_t games_played)
        :net(net), score(score), games_played(games_played) { }

    ScoredNetwork(const ScoredNetwork & other) = default;
};


void adjustScore(int winner, std::vector<ScoredNetwork> & pool, size_t i, size_t j);
void evolveNetworks(std::vector<ScoredNetwork> & pool);
size_t getRandomOpponentIndex(size_t i, size_t size);
void loadPoolFrom(const char * filename, std::vector<ScoredNetwork> & pool);
int playGame(Network4 & red_net, Network4 & black_net);
void randomizePool(std::vector<ScoredNetwork> & pool);


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
        auto result = min_max_search_inplace(board, _color_id, 4);
        return result.first;
    }
};

static std::default_random_engine generator;

int main(int argc, char const *argv[]) {
    std::vector<ScoredNetwork> pool;
    pool.reserve(STARTING_POOL_SIZE);

    if (argc > 1) {
        const char * filename = argv[1];
        std::cout << "Resuming training from file..." << '\n';
        loadPoolFrom(filename, pool);
    }
    else {
        randomizePool(pool);
    }

    for (size_t generation = 1; generation < GENERATION_TARGET + 1; generation++) {
        std::cout << "Starting generation " << generation << '\n';
        std::cout << "Playing games..." << '\n';
        // TODO: Add timing code
        for (size_t i = 0; i < pool.size(); i++) {
            auto net = pool[i].net;
            for (size_t c = pool[i].games_played; c < NUM_GAMES; c++) {
                auto j = getRandomOpponentIndex(i, pool.size());
                auto net_opponent = pool[i].net;
                auto winner = playGame(net, net_opponent);

                adjustScore(winner, pool, i, j);
                pool[i].games_played++;
                pool[j].games_played++;
            }
        }
        std::cout << "Evolving..." << '\n';
        evolveNetworks(pool);
        for (size_t i = 0; i < pool.size(); i++) {
            pool[i].score = 0;
            pool[i].games_played = 0;
        }
    }

    return 0;
}


void loadPoolFrom(const char * filename, std::vector<ScoredNetwork> & pool) {
    throw NotImplementedException();
}


void randomizePool(std::vector<ScoredNetwork> & pool) {
    for (size_t i = 0; i < STARTING_POOL_SIZE; i++) {
        pool.emplace_back(TOPOLOGY, 0, 0);
        pool[pool.size() - 1].net.randomizeWeights();
    }
}


size_t getRandomOpponentIndex(size_t i, size_t size) {
    size_t result = i;
    std::uniform_int_distribution<int> distribution(0, size);

    while (result != i) {
        result = distribution(generator);
    }
    return result;
}


int playGame(Network4 & red_net, Network4 & black_net) {
    AIPlayer p1(RED_PLAYER, red_net);
    AIPlayer p2(BLACK_PLAYER, black_net);

    Game game(p1, p2);
    game.playGame();
    return game.getWinner();
}


void adjustScore(int winner, std::vector<ScoredNetwork> & pool, size_t i, size_t j) {
    if (winner == RED_PLAYER) {
        pool[i].score += WIN_POINTS;
        pool[j].score += LOSS_POINTS;
    }
    else if (winner == BLACK_PLAYER) {
        pool[i].score += LOSS_POINTS;
        pool[j].score += WIN_POINTS;
    }
    else {
        pool[i].score += DRAW_POINTS;
        pool[j].score += DRAW_POINTS;
    }
}

void evolveNetworks(std::vector<ScoredNetwork> & pool) {
    std::sort(pool.begin(), pool.end(),
        [&](const ScoredNetwork & a, const ScoredNetwork & b) {
            return a.score < b.score;
        }
    );

    std::vector<ScoredNetwork> survivors;
    survivors.reserve(SURVIVAL_CUTTOFF);

    for (size_t i = 0; i < SURVIVAL_CUTTOFF; i++) {
        survivors.emplace_back(pool[i]);
    }
    pool.clear();
    pool.reserve(NUM_OFFSPRING * survivors.size());

    std::cout << "creating children" << '\n';
    for (size_t i = 0; i < survivors.size(); i++) {
        for (size_t c = 0; c < NUM_OFFSPRING; c++) {
            Network4 child = survivors[i].net;
            child.evolve();
            pool.emplace_back(child, 0, 0);
        }
    }
    std::cout << "done" << '\n';
}
