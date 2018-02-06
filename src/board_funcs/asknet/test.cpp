


#include "asknet.h"
#include <vector>
#include <iostream>
#include <chrono>


int main() {

    // std::vector<Node> layer1_nodes;
    // layer1_nodes.push_back(Node({1.0f}));
    // layer1_nodes.push_back(Node({1.0f}));
    // layer1_nodes.push_back(Node({1.0f}));
    //
    // std::vector<Node> layer2_nodes;
    // layer2_nodes.push_back(Node({1.0f, 1.0f, 1.0f}));
    //
    // std::vector<Layer> layers;
    // layers.push_back(Layer(layer1_nodes));
    // layers.push_back(Layer(layer2_nodes));
    //
    // Network net(layers);
    //
    // std::vector<float> inputs = {1.0f, 1.0f, 1.0f};

    Network net2({ 32, 64, 128, 7, 1 });
    net2.randomizeWeights();
    std::vector<float> inputs2(32, 1.0f);
    auto start = std::chrono::high_resolution_clock::now();
    auto result = net2.evaluate(inputs2);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

    std::cout << "Network2 size: " << net2.getNumNodes() << " nodes" << '\n';
    std::cout << "Network2 output: " << result << " in " << elapsed << "ns\n";
}
