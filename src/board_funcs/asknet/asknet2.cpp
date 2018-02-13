// asknet2.cpp
// Rohan Weeden
// Created: Feb. 10, 2018

// Implementation for AskNet2 fast cpu neural network

#include "asknet2.h"
#include "immintrin.h"
#include <cmath>
#include <cstddef>
#include <random>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

Network2::Network2(const std::vector<size_t> &topology){
    std::cout << "Network2 constructor" << '\n';
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
    std::cout << "Done" << '\n';
}

float Network2::evaluate(const std::vector<float> &inputs) {
    std::cout << "Network2 evaluate" << '\n';
    if (_layers.size() == 0) { return 0.0; }
    // Special case for first layer
    auto input_size = inputs.size();
    const float * layer_outputs = &inputs[0];

    for (size_t i = 0; i < _layers.size(); i++) {
        _layers[i].evaluate(layer_outputs, input_size);
        layer_outputs = _layers[i].getOutputs();
        input_size = _layers[i].getOutputSize();
    }

    return layer_outputs[0];
}

void Network2::randomizeWeights() {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dist(-1, 1);
    for (size_t i = 0; i < _layers.size(); i++) {
        auto layer_weights = _layers[i].getWeights();
        for (size_t j = 0; j < _layers[i].getWeightsSize(); j++) {
            float r = dist(engine);
            layer_weights[j] = r;
        }
    }
}

void Network2::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
    auto num_layers = _layers.size();
    if (weights.size() != num_layers) {
        throw std::out_of_range("Wrong number of layers (" + std::to_string(weights.size()) + ") passed to setWeights" );
    }
    for (size_t i = 0; i < num_layers; i++) {
        auto l_weights = _layers[i].getWeights();
        auto block_size = _layers[i].getBlockSize();
        auto num_weights = block_size;
        auto num_nodes = _layers[i].size();
        if (weights[i].size() != num_nodes) {
            throw std::out_of_range("Wrong number of nodes (" + std::to_string(weights[i].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + ")" );
        }
        for (size_t j = 0; j < num_nodes; j++) {
            if (weights[i][j].size() != num_weights) {
                throw std::out_of_range("Wrong number of weights (" + std::to_string(weights[i][j].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + "), Node (" + std::to_string(j) + ")" );
            }
            for (size_t k = 0; k < num_weights; k++) {
                l_weights[j * block_size + k] = weights[i][j][k];
            }
        }
    }
}

size_t Network2::getNumNodes() {
    size_t numNodes = 0;
    for (size_t i = 0; i < _layers.size(); i++) {
        numNodes += _layers[i].size() / _layers[i].getBlockSize();
    }
    return numNodes;
}

Layer2::Layer2(std::vector<Node2> nodes) {
    auto num_nodes = nodes.size();
    _weight_block_size = nodes[0].size();
    _weights_size = _weight_block_size * num_nodes;
    _layer_mem_size = _weights_size + num_nodes;
    _layer_mem = new float[_layer_mem_size];
    _weights = _layer_mem;

    for (size_t i = 0; i < num_nodes; i++) {
        auto node = nodes[i];
        auto node_size = node.array_size();
        auto index_offset = i * node_size;
        for (size_t j = 0; j < node_size; j++) {
            _weights[index_offset + j] = node.getWeight(j);
        }
    }
    _outputs = &_weights[_weights_size];
    _output_size = num_nodes;
}

Layer2::~Layer2() {
    delete[] _layer_mem;
}

void Layer2::evaluate(const float * inputs, size_t input_size) {
    for (size_t i = 0; i < input_size; i++) {
        float input = inputs[i];
        for (size_t j = 0; j < _weight_block_size; j++) {
            _outputs[j] += _weights[j] * input;
        }
    }
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
    delete[] _weights;
    delete[] _output_temp;
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

    for (size_t i = 0; i < _array_size; i+=8) {
        // SIMD multiply... Doesn't actually seem to help much
        __m256 sse_in = _mm256_load_ps(&inputs[i]);
        __m256 sse_wt = _mm256_load_ps(&_weights[i]);
        __m256 sse_out = _mm256_load_ps(&_output_temp[i]);
        sse_out = _mm256_mul_ps(sse_in, sse_wt);
        _mm256_store_ps(&_output_temp[i], sse_out);
    }
    for (size_t i = 0; i < _size; i++) {
        output += _output_temp[i];
    }
    return output;
}

float Node2::_applySigmoid(float num) {
    return std::tanh(num);
}
