
#include "asknet4.h"
#include <vector>
#include <iostream>

int main(int argc, char const *argv[]) {

    Network4 net3({3, 3, 1});
    auto data = net3.getData();
    std::cout << "data: " << '\n';
    for (size_t i = 0; i < data.size(); i++) {
        std::cout  << data[i] << " ";
    }
    std::cout << '\n';
    return 0;
}
