


#include "asknet.h"
#include "asknet2.h"
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

    std::cout << "Creating Network..." << '\n';
    Network net2({32, 10000, 1000, 500, 100, 50, 1 });
    net2.randomizeWeights();
    // net2.setWeights({
    //     { {2.0f}, {3.0f}, {4.0f} },
    //     { {2.0f, 3.0f, 4.0f}, {5.0f, 6.0f, 8.0f} },
    //     { {2.0f, 3.0f, 4.0} }
    // });
    std::cout << "Creating Network..." << '\n';

    Network net3({32, 10000, 1000, 500, 100, 50, 1 });
    net3.randomizeWeights();

    std::vector<float> inputs3(32, 1.0f);
    std::cout << "Evaluating..." << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    auto result = net3.evaluate(inputs3);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

    std::cout << "Network3 size: " << net3.getNumNodes() << " nodes" << '\n';
    std::cout << "Network3 output: " << result << " in " << elapsed << "ns\n";

    std::vector<float> inputs2(32, 1.0f);
    std::cout << "Evaluating..." << '\n';
    start = std::chrono::high_resolution_clock::now();
    result = net2.evaluate(inputs2);
    end = std::chrono::high_resolution_clock::now();
    elapsed = ((std::chrono::nanoseconds)(end - start)).count();

    std::cout << "Network2 size: " << net2.getNumNodes() << " nodes" << '\n';
    std::cout << "Network2 output: " << result << " in " << elapsed << "ns\n";

}
