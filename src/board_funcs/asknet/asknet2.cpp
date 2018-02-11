// asknet2.cpp
// Rohan Weeden
// Created: Feb. 10, 2018

// Implementation for AskNet2 fast cpu neural network

#include "asknet2.h"
#include <cmath>
#include <cstddef>
#include <random>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

Network2::Network2(const std::vector<size_t> &topology){
    auto num_layers = topology.size();
    _layers = std::vector<Layer2>();

    size_t prev_num_nodes = 1;
    for (size_t i = 0; i < num_layers; i++) {
        auto num_nodes = topology[i];
        std::vector<float> weights(prev_num_nodes, 0.0f);
        std::vector<Node2> nodes(num_nodes, weights);

        _layers.push_back(Layer2(nodes));
        prev_num_nodes = num_nodes;
    }
}

float Network2::evaluate(const std::vector<float> &inputs) {
    if (_layers.size() == 0) { return 0.0; }
    // Special case for first layer
    auto layer_outputs = std::make_shared<std::vector<float>>(
        std::vector<float>(_layers[0].evaluateFirst(inputs))
    );

    if (_layers.size() == 1) {
        (*layer_outputs)[0];
    }

    for (size_t i = 1; i < _layers.size(); i++) {
        layer_outputs = std::make_shared<std::vector<float>>(
            _layers[i].evaluate(*layer_outputs)
        );
    }

    return (*layer_outputs)[0];
}

void Network2::randomizeWeights() {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dist(-1, 1);
    for (size_t i = 0; i < _layers.size(); i++) {
        for (size_t j = 0; j < _layers[i].size(); j++) {
            auto node = &_layers[i].getNodes()[j];
            for (size_t k = 0; k < node->size(); k++) {
                float r = dist(engine);
                node->setWeight(k, r);
            }
        }
    }
}

void Network2::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
    auto network_size = _layers.size();
    if (weights.size() != network_size) {
        throw std::out_of_range("Wrong number of layers (" + std::to_string(weights.size()) + ") passed to setWeights" );
    }
    for (size_t i = 0; i < network_size; i++) {
        auto layer_size = _layers[i].size();
        if (weights[i].size() != layer_size) {
            throw std::out_of_range("Wrong number of nodes (" + std::to_string(weights[i].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + ")" );
        }
        for (size_t j = 0; j < layer_size; j++) {
            auto node = &_layers[i].getNodes()[j];
            auto node_size = node->size();
            if (weights[i][j].size() != node_size) {
                throw std::out_of_range("Wrong number of weights (" + std::to_string(weights[i][j].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + "), Node (" + std::to_string(j) + ")" );
            }
            for (size_t k = 0; k < node_size; k++) {
                node->setWeight(k, weights[i][j][k]);
            }
        }
    }
}

size_t Network2::getNumNodes() {
    size_t numNodes = 0;
    for (size_t i = 0; i < _layers.size(); i++) {
        numNodes += _layers[i].size();
    }
    return numNodes;
}

std::vector<float> Layer2::evaluate(const std::vector<float> &inputs) {
    std::vector<float> outputs;
    for (size_t i = 0; i < _nodes.size(); i++) {
        outputs.push_back(_nodes[i].evaluate(inputs));
    }
    return outputs;
}

std::vector<float> Layer2::evaluateFirst(const std::vector<float> &inputs) {
    auto layer_size = _nodes.size();
    if (inputs.size() != layer_size) {
        throw std::out_of_range("Must pass exactly " + std::to_string(layer_size) + " weights to Layer");
    }

    std::vector<float> output(layer_size);
    std::vector<float> input(1);
    for (size_t i = 0; i < layer_size; i++) {
        input[0] = inputs[i];
        output[i] = _nodes[i].evaluate(input);
    }
    return output;
}

Node2::Node2(const std::vector<float> & weights) {
    _size = weights.size();
    _array_size = _size + weights.size() % 8;
    _weights = new float[_array_size];
    _output_temp = new float[_array_size];
    for (size_t i = 0; i < _size; i++) {
        _weights[i] = weights[i];
    }
}

Node2::~Node2() {
    free(_weights);
    free(_output_temp);
}

float Node2::evaluate(const std::vector<float> &inputs) {
    if (inputs.size() != _size) {
        throw std::out_of_range("Must pass exactly " + std::to_string(_size) + " weights to Node");
    }

    float sum = _sumWeights(&inputs[0]);
    return _applySigmoid(sum);
}

float Node2::_sumWeights(const float * inputs) {
    float output = 0.0f;

    for (size_t i = 0; i < _array_size; i+=4) {
        // SIMD multiply... Doesn't actually seem to help much
        __m128 sse_in = _mm_load1_ps(&inputs[i]);
        __m128 sse_wt = _mm_load1_ps(&_weights[i]);
        __m128 sse_out = _mm_load_ps(&_output_temp[i]);
        sse_out = _mm_mul_ps(sse_in, sse_wt);
        _mm_store_ps(&_output_temp[i], sse_out);
    }
    for (size_t i = 0; i < _size; i++) {
        output += _output_temp[i];
    }
    return output;
}

float Node2::_applySigmoid(float num) {
    return std::tanh(num);
}
