// asknet3.cpp
// Rohan Weeden
// Created: Feb. 13, 2018

// Implementation for AskNet v4. Check the header file for documentation

#include "asknet4.h"
#include <cmath>
#include <vector>
#include <cstddef>
#include <iostream>
#include <stdexcept>


Network4::Network4(const std::vector<size_t> & topology) {
    size_t required_space = _getRequiredSpace(topology);
    _num_layers = topology.size();

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
        auto space = _getLayerRequiredSpace(num_nodes_in_layer, num_node_inputs);
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

LayerHeader Network4::_readLayerHeader(float * start) {
    return { start[0], start[1], start[2], start[3] };
}

void Network4::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    size_t i = 0;
    while (layerStart < dataEnd) {
        if (! (i < weights.size())) {
            throw std::out_of_range("Wrong number of layers (" + std::to_string(weights.size()) + ") passed to setWeights" );
        }
        auto header = _readLayerHeader(layerStart);
        if (weights[i].size() != header.num_nodes) {
            throw std::out_of_range("Wrong number of nodes (" + std::to_string(weights[i].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + ")" );
        }
        for (size_t j = 0; j < header.num_nodes; j++) {
            if (weights[i][j].size() != header.num_node_weights) {
                throw std::out_of_range("Wrong number of weights (" + std::to_string(weights[i][j].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + "), Node (" + std::to_string(j) + ")" );
            }
            for (size_t k = 0; k < header.num_node_weights; k++) {
                layerStart[(size_t) header.size + ((size_t) header.node_size * j) + k] = weights[i][j][k];
            }
        }
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
        i++;
    }
}

std::vector<std::vector<std::vector<float>>> Network4::getWeights() {
    std::vector<std::vector<std::vector<float>>> weights;
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    size_t i = 0;
    while (layerStart < dataEnd) {
        if (! (i < weights.size())) {
            throw std::out_of_range("Wrong number of layers (" + std::to_string(weights.size()) + ") passed to setWeights" );
        }
        auto header = _readLayerHeader(layerStart);
        if (weights[i].size() != header.num_nodes) {
            throw std::out_of_range("Wrong number of nodes (" + std::to_string(weights[i].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + ")" );
        }
        std::vector<std::vector<float>> layer;
        for (size_t j = 0; j < header.num_nodes; j++) {
            if (weights[i][j].size() != header.num_node_weights) {
                throw std::out_of_range("Wrong number of weights (" + std::to_string(weights[i][j].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + "), Node (" + std::to_string(j) + ")" );
            }
            std::vector<float> node;
            for (size_t k = 0; k < header.num_node_weights; k++) {
                node.push_back(layerStart[(size_t) header.size + ((size_t) header.node_size * j) + k]);
            }
            layer.push_back(node);
        }
        weights.push_back(layer);
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
        i++;
    }
    return weights;
}

// TODO: Refactor this......
float Network4::evaluate(const std::vector<float> & inputs) {
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    std::vector<float> layer_outputs(inputs.size());

    auto header = _readLayerHeader(layerStart);
    if (header.num_nodes != layer_outputs.size()) {
        throw std::out_of_range("Wrong number of inputs (" + std::to_string(layer_outputs.size()) + ") passed to evaluate. Expecting " + std::to_string((size_t) header.num_nodes ));
    }
    for (size_t i = 0; i < inputs.size(); i++) {
        float output = layerStart[(size_t)header.size + ((size_t) header.node_size * i)] * inputs[i];
        output = _applySigmoid(output);
        layer_outputs[i] = output;
    }
    layerStart += (size_t) (header.size + header.num_nodes * header.node_size);

    size_t i = 1;
    while (layerStart < dataEnd) {
        // Does a copy
        std::vector<float> layer_inputs(layer_outputs);
        auto layer_inputs_size = layer_inputs.size();

        header = _readLayerHeader(layerStart);
        if (header.num_node_weights != layer_inputs_size) {
            throw std::out_of_range("Wrong number of inputs (" + std::to_string(layer_inputs_size) + ") passed to evaluate. Expecting " + std::to_string((size_t) header.num_node_weights ));
        }
        layer_outputs.resize((size_t) header.num_nodes);

        for (size_t j = 0; j < header.num_nodes; j++) {
            float node_output = 0.0f;
            for (size_t k = 0; k < header.num_node_weights; k++) {
                node_output += (layerStart[(size_t) header.size + ((size_t) header.node_size * j) + k] * layer_inputs[k]);
            }
            node_output = _applySigmoid(node_output);
            layer_outputs[j] = node_output;
        }
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
        i++;
    }
    return layer_outputs[0];
}


float Network4::_applySigmoid(float num) {
    return tanh(num);
}
