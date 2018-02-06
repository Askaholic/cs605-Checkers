


#include "asknet.h"
#include <vector>
#include <iostream>


int main() {

    std::vector<Node> layer1_nodes;
    layer1_nodes.push_back(Node({0.5f}));
    layer1_nodes.push_back(Node({0.2f}));
    layer1_nodes.push_back(Node({0.6f}));

    std::vector<Node> layer2_nodes;
    layer2_nodes.push_back(Node({0.5f, 0.1f, 0.6f}));

    std::vector<Layer> layers;
    layers.push_back(Layer(layer1_nodes));
    layers.push_back(Layer(layer2_nodes));

    Network net(layers);

    std::vector<float> inputs = {1.0f, 0.0f, 1.0f};

    Network net2({ 32, 64, 1 });
    net2.randomizeWeights();
    std::vector<float> inputs2(32, 1.0f);

    std::cout << "Network output: " << net.evaluate(inputs) << '\n';
    std::cout << "Network2 output: " << net2.evaluate(inputs2) << '\n';
}
