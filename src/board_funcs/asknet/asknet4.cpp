// asknet3.cpp
// Rohan Weeden
// Created: Feb. 13, 2018

// Implementation for AskNet v4. Check the header file for documentation

#include "asknet4.h"
#include <vector>
#include <cstddef>
#include <iostream>


Network4::Network4(const std::vector<size_t> & topology) {
    size_t required_space = _getRequiredSpace(topology);

    _data = std::vector<float>(required_space, 0.0f);

    auto layer_start = &_data[0];
    size_t num_node_inputs = 1;
    for (size_t i = 0; i < topology.size(); i++) {
        auto num_nodes_in_layer = topology[i];

        layer_start = _writeLayerHeader(layer_start, num_nodes_in_layer, num_node_inputs);
        num_node_inputs = num_nodes_in_layer;
    }
}

float * Network4::_writeLayerHeader(float * start, size_t num_nodes, size_t num_node_weights) {
    size_t node_size = _getNodeRequiredSpace(num_node_weights);
    size_t total_size = _getLayerRequiredSpace(num_nodes, num_node_weights);

    start[0] = LAYER_HEADER_SIZE;
    start[1] = num_nodes;
    start[2] = node_size;
    start[3] = num_node_weights;

    float * end = start + total_size;
    return end;
}

size_t Network4::_getRequiredSpace(const std::vector<size_t> & topology) {
    size_t required_space = 0;

    size_t num_node_inputs = 1;
    for (size_t i = 0; i < topology.size(); i++) {
        auto num_nodes_in_layer = topology[i];
        std::cout << "Layer: " << num_nodes_in_layer << '\n';
        std::cout << "\tinputs: " << num_node_inputs << '\n';
        auto space = _getLayerRequiredSpace(num_nodes_in_layer, num_node_inputs);
        std::cout << "\tspace: " << space << '\n';
        required_space += space;

        num_node_inputs = num_nodes_in_layer;
    }

    return required_space;
}

size_t Network4::_getLayerRequiredSpace(size_t num_nodes, size_t num_node_weights) {
    size_t node_size = _getNodeRequiredSpace(num_node_weights);
    size_t layer_header_size = LAYER_HEADER_SIZE;
    return layer_header_size + (num_nodes * node_size);
}

size_t Network4::_getNodeRequiredSpace(size_t num_weights) {
    return num_weights;
}
