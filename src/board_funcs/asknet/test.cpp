


#include "asknet.h"
#include "asknet2.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

long time_network(Network net, std::string name, std::vector<float> inputs, size_t iterations) {
    std::cout << "Evaluating " << name << " ..." << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    float result;
    for (size_t i = 0; i < iterations; i++) {
        result = net.evaluate(inputs);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

    std::cout << "Network " << name << " size: " << net.getNumNodes() << " nodes" << '\n';
    std::cout << "Network " << name << " output: " << result << " in " << elapsed / iterations << "ns / evaluation\n";
    return elapsed;
}

long time_network(Network2 net, std::string name, std::vector<float> inputs, size_t iterations) {
    std::cout << "Evaluating " << name << " ..." << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    float result;
    for (size_t i = 0; i < iterations; i++) {
        result = net.evaluate(inputs);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

    std::cout << "Network " << name << " size: " << net.getNumNodes() << " nodes" << '\n';
    std::cout << "Network " << name << " output: " << result << " in " << elapsed / iterations << "ns / evaluation\n";
    return elapsed;
}

void test_topology(std::vector<size_t> topology, size_t NUM_TESTS) {
    std::string topoStr = "";
    for (size_t i = 0; i < topology.size(); i++) {
        topoStr += " " + std::to_string(topology[i]);
    }
    std::cout << "*****************" << '\n' << topoStr << '\n' << "*****************" << '\n';
    Network net2(topology);
    net2.randomizeWeights();

    Network2 net3(topology);
    net3.setWeights(net2.getWeights());

    std::vector<float> inputs(32, 0.001f);
    auto elapsed = time_network(net3, "(3)", inputs, NUM_TESTS);

    std::cout << 1000000000 * NUM_TESTS / (double) elapsed << " eval / second" << '\n';

    elapsed = time_network(net2, "(2)", inputs, NUM_TESTS);
    std::cout << 1000000000.0 * NUM_TESTS / (double) elapsed << " eval / second" << '\n';

}

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
    size_t NUM_TESTS = 100;

    test_topology({32, 10000, 1 }, NUM_TESTS);
    test_topology({32, 5000, 5000, 1 }, NUM_TESTS);
    test_topology({32, 5000, 3000, 2000, 1 }, NUM_TESTS);
    test_topology({32, 2000, 3000, 3000, 2000, 1 }, NUM_TESTS);
    test_topology({32, 5000, 3000, 1000, 1000, 1 }, NUM_TESTS);

}
