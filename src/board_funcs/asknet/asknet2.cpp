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
    auto num_layers = topology.size();
    _layers = std::vector<Layer2>();

    size_t prev_num_nodes = 1;
    for (size_t i = 0; i < num_layers; i++) {
        auto num_nodes = topology[i];
        std::cout << "Num nodes: " << num_nodes << '\n';
        std::vector<float> weights(prev_num_nodes, 0.0f);
        std::vector<Node2> nodes(num_nodes, Node2(weights));

        std::cout << "nodes.size() " << nodes.size() << '\n';
        _layers.push_back(nodes);
        prev_num_nodes = num_nodes;
    }
}

float Network2::evaluate(const std::vector<float> &inputs) {
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
        auto num_weights = _layers[i].size();
        auto num_nodes = _layers[i].getOutputSize();
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
    std::cout << _layers.size() << " layers" << '\n';
    for (size_t i = 0; i < _layers.size(); i++) {
        numNodes += _layers[i].getOutputSize();
    }
    return numNodes;
}

Layer2::Layer2(std::vector<Node2> nodes) {
    auto num_nodes = nodes.size();
    _weight_block_size = num_nodes;
    _weights_size = _weight_block_size * nodes[0].size();
    _layer_mem_size = _weights_size + num_nodes;
    _layer_mem = new float[_layer_mem_size];
    _weights = &_layer_mem[0];
    _outputs = &_weights[_weights_size];
    _output_size = num_nodes;

    for (size_t i = 0; i < num_nodes; i++) {
        auto node = nodes[i];
        auto node_size = node.size();
        auto index_offset = i * node_size;
        for (size_t j = 0; j < node_size; j++) {
            _weights[index_offset + j] = node.getWeight(j);
        }
    }
}

Layer2::Layer2(const Layer2 & layer) {
    _weight_block_size = layer._weight_block_size;
    _weights_size = layer._weights_size;
    _layer_mem_size = layer._layer_mem_size;
    _output_size = layer._output_size;

    _layer_mem = new float[_layer_mem_size];
    _weights = &_layer_mem[0];
    _outputs = &_weights[_weights_size];

    std::copy(layer._layer_mem, layer._layer_mem + _layer_mem_size, _layer_mem);
}

Layer2::Layer2(const Layer2 && layer) {
    *this = std::move(layer);
}


Layer2 & Layer2::operator=(const Layer2 & layer) {
    if (this == &layer) {
        return *this;
    }

    _weight_block_size = layer._weight_block_size;
    _weights_size = layer._weights_size;
    _layer_mem_size = layer._layer_mem_size;
    _output_size = layer._output_size;

    _layer_mem = new float[_layer_mem_size];
    _weights = &_layer_mem[0];
    _outputs = &_weights[_weights_size];

    std::copy(layer._layer_mem, layer._layer_mem + _layer_mem_size, _layer_mem);

    return *this;
}

Layer2 & Layer2::operator=(Layer2 && layer) {
    if (this == &layer) {
        return *this;
    }

    _weight_block_size = layer._weight_block_size;
    _weights_size = layer._weights_size;
    _layer_mem_size = layer._layer_mem_size;
    _output_size = layer._output_size;

    _layer_mem = layer._layer_mem;
    layer._layer_mem = nullptr;

    _weights = &_layer_mem[0];
    _outputs = &_weights[_weights_size];

    return *this;
}

Layer2::~Layer2() {
    if (_layer_mem != nullptr) {
        delete[] _layer_mem;
    }
    _weights = nullptr;
    _outputs = nullptr;
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
    _array_size = _size + (weights.size() % 16);
    _weights = new float[_array_size];
    _output_temp = new float[_array_size];
    for (size_t i = 0; i < _size; i++) {
        _weights[i] = weights[i];
    }
}

Node2::Node2(const Node2 & node) {
    // std::cout << "copy constructor" << '\n';
    _size = node._size;
    _array_size = node._array_size;
    _weights = new float[_array_size];
    _output_temp = new float[_array_size];

    std::copy(node._weights, node._weights + _array_size, _weights);
    std::copy(node._output_temp, node._output_temp + _array_size, _output_temp);
}

Node2::Node2(Node2 && node) {
    std::cout << "move constructor" << '\n';
    *this = std::move(node);
}

Node2 & Node2::operator=(const Node2 & node) {
    if (this == &node) {
        return *this;
    }

    delete[] _weights;
    delete[] _output_temp;

    _size = node._size;
    _array_size = node._array_size;
    _weights = new float[_array_size];
    _output_temp = new float[_array_size];

    std::copy(node._weights, node._weights + _array_size, _weights);
    std::copy(node._output_temp, node._output_temp + _array_size, _output_temp);

    return *this;
}

Node2 & Node2::operator=(Node2 && node) {
    if (this == &node) {
        return *this;
    }

    _size = node._size;
    _array_size = node._array_size;

    _weights = node._weights;
    _output_temp = node._output_temp;

    node._weights = nullptr;
    node._output_temp = nullptr;

    return *this;
}

Node2::~Node2() {
    if (_weights != nullptr) {
        delete[] _weights;
    }
    if (_output_temp != nullptr) {
        delete[] _output_temp;
    }
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
