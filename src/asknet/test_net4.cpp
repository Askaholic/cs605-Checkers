
#include "asknet4.h"
#include <vector>
#include <iostream>

int main(int argc, char const *argv[]) {

    Network4 net4({3, 3, 1});
    net4.setWeights({
        {{2}, {1}, {2}},
        {{1, 1, 1}, {.4, .4, .4}, {.2, .2, .2}},
        {{1, 2, 1}}
    });
    auto data = net4.getData();
    // std::cout << "data: " << '\n';
    // for (size_t i = 0; i < data.size(); i++) {
    //     std::cout  << data[i] << " ";
    // }
    // std::cout << '\n';
    std::cout << "Result: " << net4.evaluate({1, 1, 1}) << '\n';
    return 0;
}
