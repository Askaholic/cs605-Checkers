// asknet.cpp
// Rohan Weeden
// Created: Feb. 2, 2018

// Implementation for AskNet neural network

#include "asknet.h"
#include <vector>
#include <cstddef>


Layer::Layer(size_t num_nodes) {
    _weights = {0.0}
}
