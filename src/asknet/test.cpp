

#include "aligned_array.h"
#include "asknet.h"
// #include "asknet3.h"
// #include "asknet4.h"
#include "jnet.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

// long time_network(Network & net, const std::string & name, const std::vector<float> & inputs, size_t iterations) {
//     std::cout << "Evaluating " << name << " ..." << '\n';
//     std::cout << "Network " << name << " size: " << net.getNumNodes() << " nodes" << '\n';

//     float result;
//     auto start = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < iterations; i++) {
//         result = net.evaluate(inputs);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

//     std::cout << "Network " << name << " output: " << result << " in " << elapsed / iterations << "ns / evaluation\n";
//     return elapsed;
// }

// long time_network(Network3 & net, const std::string & name, const std::vector<float> & inputs, size_t iterations) {
//     std::cout << "Evaluating " << name << " ..." << '\n';
//     std::cout << "Network " << name << " size: " << net.getNumNodes() << " nodes" << '\n';

//     float result;
//     auto start = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < iterations; i++) {
//         result = net.evaluate(inputs);
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

//     std::cout << "Network " << name << " output: " << result << " in " << elapsed / iterations << "ns / evaluation\n";
//     return elapsed;
// }

// long time_network(Network4 & net, const std::string & name, const std::vector<float> & inputs, size_t iterations) {
//     std::cout << "Evaluating " << name << " ..." << '\n';
//     std::cout << "Network " << name << " size: " << net.getNumNodes() << " nodes" << '\n';

//     float result;
//     net.setInputs(inputs);

//     auto start = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < iterations; i++) {
//         result = net.evaluate();
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

//     std::cout << "Network " << name << " output: " << result << " in " << elapsed / iterations << "ns / evaluation\n";
//     return elapsed;
// }

long time_network(JNet & net, const std::string & name, const std::vector<float> & inputs, size_t iterations) {
    std::cout << "Evaluating " << name << " ..." << '\n';
    std::cout << "Network " << name << " size: " << net.getNumNodes() << " nodes" << '\n';

    float result;
    net.setInputs(inputs);

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; i++) {
        result = net.evaluate();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = ((std::chrono::nanoseconds)(end - start)).count();

    std::cout << "Network " << name << " output: " << result << " in " << elapsed / iterations << "ns / evaluation\n";
    return elapsed;
}


void test_topology(const std::vector<size_t> & topology, size_t NUM_TESTS) {
    std::string topoStr = "";
    for (size_t i = 0; i < topology.size(); i++) {
        topoStr += " " + std::to_string(topology[i]);
    }
    std::cout << "*****************" << '\n' << topoStr << '\n' << "*****************" << '\n';
    Network net1(topology);
    net1.randomizeWeights();
    auto weights = net1.getWeights();

    // Network3 net3(topology);
    // net3.setWeights(weights);


    // Network4 net4(topology);
    // net4.setWeights(weights);

    JNet jnet(topology);
    jnet.setWeights(weights);
    // jnet.randomizeWeights();


    std::vector<float> inputs(topology[0], 0.1f);
    long elapsed;
    // auto elapsed = time_network(net3, "(3)", inputs, NUM_TESTS);
    // std::cout << 1000000000.0 * NUM_TESTS / (double) elapsed << " eval / second\n" << '\n';

    // elapsed = time_network(net1, "(2)", inputs, NUM_TESTS);
    // std::cout << 1000000000.0 * NUM_TESTS / (double) elapsed << " eval / second\n" << '\n';

    // elapsed = time_network(net4, "(4)", inputs, NUM_TESTS);
    // std::cout << 1000000000.0 * NUM_TESTS / (double) elapsed << " eval / second\n" << '\n';

    elapsed = time_network(jnet, "(jnet)", inputs, NUM_TESTS);
    std::cout << 1000000000.0 * NUM_TESTS / (double) elapsed << " eval / second\n" << '\n';

    jnet.writeNNToFile();
    jnet.readFileToNN();
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

    size_t NUM_TESTS = 100000;

    // test_topology({3, 1 }, NUM_TESTS);
    test_topology({32, 40, 10, 1 }, NUM_TESTS);
    // test_topology({32, 110, 40, 10, 1 }, NUM_TESTS);
    // test_topology({32, 10000, 1 }, NUM_TESTS);
    // test_topology({32, 5000, 5000, 1 }, NUM_TESTS);
    // test_topology({32,40,10,1}, NUM_TESTS);
    // test_topology({32, 5000, 3000, 2000, 1 }, NUM_TESTS);
    // test_topology({32, 2000, 3000, 3000, 2000, 1 }, NUM_TESTS);
    // test_topology({32, 5000, 3000, 1000, 1000, 1 }, NUM_TESTS);
    // test_topology({32, 1000, 3000, 1000, 50, 4, 1 }, NUM_TESTS);
    // test_topology({32, 3000, 1000, 500, 50, 4, 1 }, NUM_TESTS);

    // {
    //     AlignedArray<float, 32> aarr(10);
    //     std::cout << "mem: " << &aarr[0] << '\n';
    // }

}
