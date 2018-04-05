// network_test.cpp
// Rohan Weeden
// Created: March 28, 2018

// For testing the output of the neural net

#include "asknet.h"
#include "board_funcs.h"
#include "player.h"
#include <iostream>
#include <vector>


int main(int argc, char const *argv[]) {
    Network net_orig({32, 40, 10, 1});
    net_orig.randomizeWeights();
    auto weights = net_orig.getWeights();

    Network4 net({32, 40, 10, 1});
    net.setWeights(weights);

    // net.readFromFile("best_network.txt");
    // net.randomizeWeights();
    // net.writeToFile("test_net.txt");
    //
    // auto b = make_board({
    //       'r','r','r','r',
    //     'r','r','r','r',
    //       'r','r','r','r',
    //     ' ',' ',' ',' ',
    //       ' ',' ',' ',' ',
    //     'b','b','b','b',
    //       'b','b','b','b',
    //     'b','b','b','b',
    // });
    //
    // AIPlayer player(RED_PLAYER, net);
    // std::cout << "Score: " << player.evaluate(b, RED_PLAYER)  << '\n';
    std::vector<float> inputs;
    for (size_t i = 0; i < 32; i++) {
        inputs.push_back(.5);
    }
    std::cout << "Score: " << net_orig.evaluate(inputs) << '\n';
    net.setInputs(inputs);
    std::cout << "Score: " << net.evaluate() << '\n';

    return 0;
}
