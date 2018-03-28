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


class ScoredNetwork {
public:
    Network4 net;
    int score;
    size_t games_played;
    std::string id = "_";

public:
    ScoredNetwork(std::vector<size_t> topology, int score, size_t games_played)
        :net(topology), score(score), games_played(games_played) { }

    ScoredNetwork(const Network4 & net, int score, size_t games_played)
        :net(net), score(score), games_played(games_played) { }

    ScoredNetwork() = delete;
    ScoredNetwork(const ScoredNetwork & other) = default;
    ScoredNetwork(ScoredNetwork && other) = default;
    ScoredNetwork & operator=(const ScoredNetwork & other) = default;
    ScoredNetwork & operator=(ScoredNetwork && other) = default;
    virtual ~ScoredNetwork() = default;
};


void adjustScore(int winner, std::vector<ScoredNetwork> & pool, size_t i, size_t j);
void evolveNetworks(std::vector<ScoredNetwork> & pool, size_t generation);
size_t getRandomOpponentIndex(size_t i, size_t size);
void loadPoolFrom(const char * filename, std::vector<ScoredNetwork> & pool);
int playGame(const Network4 & red_net, const Network4 & black_net);
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

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < pool.size(); i++) {
            auto net = pool[i].net;
            for (size_t c = pool[i].games_played; c < NUM_GAMES; c++) {
                auto j = getRandomOpponentIndex(i, pool.size());
                auto net_opponent = pool[j].net;

                // std::cout << i << " vs " << j << '\n';
                auto winner = playGame(net, net_opponent);
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
    std::uniform_int_distribution<int> distribution(0, size - 1);

    while (result == i) {
        result = distribution(generator);
    }
    return result;
}


int playGame(const Network4 & red_net, const Network4 & black_net) {
    AIPlayer p1(RED_PLAYER, red_net);
    AIPlayer p2(BLACK_PLAYER, black_net);

    Game game(p1, p2);
    game.randomizeOpeningMoves(NUM_OPENING_MOVES);
    game.playGame();
    // print_board(game.getBoard());
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

void evolveNetworks(std::vector<ScoredNetwork> & pool, size_t generation) {
    for (size_t i = 0; i < pool.size(); i++) {
        if (pool[i].id == "_") {
            pool[i].id = std::to_string(generation) + "_" + std::to_string(i);
        }
    }
    std::cout << "sorting" << '\n';
    std::sort(pool.begin(), pool.end(),
        [&](const ScoredNetwork & a, const ScoredNetwork & b) {
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
    pool[0].net.writeToFile("best_network" + std::to_string(generation) + ".txt");
    // for (size_t i = 0; i < pool.size(); i++) {
    //     std::cout << "best score " << i <<": " << pool[i].score << '\n';
    // }
    // Use this for checking that the weights are actually changing
    // for (size_t i = 0; i < pool.size(); i++) {
    //     pool[i].net.printWeights();
    // }

    std::vector<ScoredNetwork> survivors;
    survivors.reserve(SURVIVAL_CUTTOFF);

    for (size_t i = 0; i < SURVIVAL_CUTTOFF; i++) {
        survivors.emplace_back(pool[i]);
    }
    std::vector<ScoredNetwork> newPool;
    newPool.reserve(SURVIVAL_CUTTOFF + NUM_OFFSPRING * SURVIVAL_CUTTOFF);
    for (size_t i = 0; i < survivors.size(); i++) {
        newPool.emplace_back(survivors[i]);
    }

    std::cout << "creating children" << '\n';
    for (size_t i = 0; i < survivors.size(); i++) {
        for (size_t c = 0; c < NUM_OFFSPRING; c++) {
            Network4 child = survivors[i].net;
            child.evolve();
            newPool.emplace_back(child, 0, 0);
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
