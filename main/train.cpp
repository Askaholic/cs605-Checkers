// train.cpp
// Rohan Weeden
// Created: March 8, 2018

// Plays AI against eachother and evolves them.

#include "game.h"
#include "asknet4.h"
#include "search.h"
#include "board_funcs.h"
#include <algorithm>
#include <chrono>
#include <initializer_list>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#define STARTING_POOL_SIZE 50
/* Must be less than STARTING_POOL_SIZE.
 * Describes the number of networks that survive to the next generation.
 */
#define SURVIVAL_CUTTOFF 25
#define NUM_OFFSPRING 1
#define GENERATION_TARGET 300
#define NUM_GAMES 10
#define NUM_OPENING_MOVES 3
#define WIN_POINTS 1
#define LOSS_POINTS -2
#define DRAW_POINTS 0
#define TOPOLOGY (std::initializer_list<size_t>){32, 40, 10, 1}


class ScoredNetworkGroup {
public:
    Network4 beg;
    Network4 mid;
    Network4 end;

    int score;
    size_t games_played;
    std::string id = "_";

public:
    ScoredNetworkGroup(std::vector<size_t> topology, int score, size_t games_played)
        :beg(topology), mid(topology), end(topology), score(score),
        games_played(games_played) { }

    ScoredNetworkGroup(const Network4 & beg, const Network4 & mid,
                       const Network4 & end, int score, size_t games_played)
        :beg(beg), mid(mid), end(end), score(score), games_played(games_played) { }

    ScoredNetworkGroup() = delete;
    ScoredNetworkGroup(const ScoredNetworkGroup & other) = default;
    ScoredNetworkGroup(ScoredNetworkGroup && other) = default;
    ScoredNetworkGroup & operator=(const ScoredNetworkGroup & other) = default;
    ScoredNetworkGroup & operator=(ScoredNetworkGroup && other) = default;
    virtual ~ScoredNetworkGroup() = default;
};

void adjustScore(int winner, std::vector<ScoredNetworkGroup> & pool, size_t i, size_t j);
void evolveNetworks(std::vector<ScoredNetworkGroup> & pool, size_t generation);
size_t getRandomOpponentIndex(size_t i, size_t size);
void loadPoolFrom(const char * filename, std::vector<ScoredNetworkGroup> & pool);
int playGame(const ScoredNetworkGroup & red_nets, const ScoredNetworkGroup & black_nets);
void randomizePool(std::vector<ScoredNetworkGroup> & pool);


class NotImplementedException : public std::exception {
public:
    virtual const char* what() const throw() {
        return "** NOT IMPLEMENTED **";
    }
};


static std::default_random_engine generator;

int main(int argc, char const *argv[]) {
    std::vector<ScoredNetworkGroup> pool;
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

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < pool.size(); i++) {
            for (size_t c = pool[i].games_played; c < NUM_GAMES; c++) {
                auto j = getRandomOpponentIndex(i, pool.size());

                // std::cout << i << " vs " << j << '\n';
                auto winner = playGame(pool[i], pool[j]);
                // std::cout << "winner: " << winner << '\n';

                adjustScore(winner, pool, i, j);
                pool[i].games_played++;
                pool[j].games_played++;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = (end - start);
        std::cout << "Took " << elapsed.count() << " seconds\n";

        std::cout << "Evolving..." << '\n';
        evolveNetworks(pool, generation);
        std::cout << "Clearing old scores" << '\n';
        for (size_t i = 0; i < pool.size(); i++) {
            pool[i].score = 0;
            pool[i].games_played = 0;
        }
    }

    return 0;
}


void loadPoolFrom(const char * filename, std::vector<ScoredNetworkGroup> & pool) {
    throw NotImplementedException();
}


void randomizePool(std::vector<ScoredNetworkGroup> & pool) {
    for (size_t i = 0; i < STARTING_POOL_SIZE; i++) {
        pool.emplace_back(TOPOLOGY, 0, 0);
        ScoredNetworkGroup * netGroup = &pool[pool.size() - 1];
        netGroup->beg.randomizeWeights();
        netGroup->mid.randomizeWeights();
        netGroup->end.randomizeWeights();
    }
}


size_t getRandomOpponentIndex(size_t i, size_t size) {
    size_t result = i;
    std::uniform_int_distribution<int> distribution(0, size - 1);

    while (result == i) {
        result = distribution(generator);
    }
    return result;
}


int playGame(const ScoredNetworkGroup & red, const ScoredNetworkGroup & black) {
    AIPlayer3Net p1(RED_PLAYER, red.beg, red.mid, red.end);
    AIPlayer3Net p2(BLACK_PLAYER, black.beg, black.mid, black.end);

    Game game(p1, p2);
    game.randomizeOpeningMoves(NUM_OPENING_MOVES);
    game.playGame();
    // print_board(game.getBoard());
    return game.getWinner();
}


void adjustScore(int winner, std::vector<ScoredNetworkGroup> & pool, size_t i, size_t j) {
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

void evolveNetworks(std::vector<ScoredNetworkGroup> & pool, size_t generation) {
    for (size_t i = 0; i < pool.size(); i++) {
        if (pool[i].id == "_") {
            pool[i].id = std::to_string(generation) + "_" + std::to_string(i);
        }
    }
    std::cout << "sorting" << '\n';
    std::sort(pool.begin(), pool.end(),
        [&](const ScoredNetworkGroup & a, const ScoredNetworkGroup & b) {
            return ((float) b.score / (float) b.games_played) <
                   ((float) a.score / (float) a.games_played);
        }
    );

    std::cout << "top 10: " << '\n';
    for (size_t i = 0; i < 10 && i < pool.size(); i++) {
        std::cout << (float) pool[i].score / (float) pool[i].games_played << ' ';
    }
    std::cout << '\n';

    std::cout << "writing best to file" << '\n';

    pool[0].beg.writeToFile("best_network_beg" + std::to_string(generation) + ".txt");
    pool[0].mid.writeToFile("best_network_mid" + std::to_string(generation) + ".txt");
    pool[0].end.writeToFile("best_network_end" + std::to_string(generation) + ".txt");
    // for (size_t i = 0; i < pool.size(); i++) {
    //     std::cout << "best score " << i <<": " << pool[i].score << '\n';
    // }
    // Use this for checking that the weights are actually changing
    // for (size_t i = 0; i < pool.size(); i++) {
    //     pool[i].net.printWeights();
    // }

    std::vector<ScoredNetworkGroup> survivors;
    survivors.reserve(SURVIVAL_CUTTOFF);

    for (size_t i = 0; i < SURVIVAL_CUTTOFF; i++) {
        survivors.emplace_back(pool[i]);
    }
    std::vector<ScoredNetworkGroup> newPool;
    newPool.reserve(SURVIVAL_CUTTOFF + NUM_OFFSPRING * SURVIVAL_CUTTOFF);
    for (size_t i = 0; i < survivors.size(); i++) {
        newPool.emplace_back(survivors[i]);
    }

    std::cout << "creating children" << '\n';
    for (size_t i = 0; i < survivors.size(); i++) {
        for (size_t c = 0; c < NUM_OFFSPRING; c++) {
            Network4 child_beg = survivors[i].beg;
            Network4 child_mid = survivors[i].mid;
            Network4 child_end = survivors[i].end;
            child_beg.evolve();
            child_mid.evolve();
            child_end.evolve();
            newPool.emplace_back(child_beg, child_mid, child_end, 0, 0);
        }
    }
    for (size_t i = 0; i < pool.size(); i++) {
        pool[i] = newPool[i];
    }

    std::cout << "ids: " << '\n';
    for (size_t i = 0; i < pool.size(); i++) {
        std::cout << pool[i].id << " ";
    }
    std::cout << '\n';
}
