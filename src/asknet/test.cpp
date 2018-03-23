

#include "aligned_array.h"
#include "asknet.h"
#include "asknet4.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <string>


int main() {
    size_t NUM_TESTS = 100000;

    Network4 test( {1, 2, 1} );
    test.setInputs({0.5});
    test.printWeights();
    test.setWeights({
        {
            {.5},
            {.5}
        },
        {
            {1, 1}
        }
    });
    std::cout << "value: " << test.evaluate() << '\n';

    // test_topology({3, 1 }, NUM_TESTS);
    // test_topology({32, 40, 10, 1 }, NUM_TESTS);
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
