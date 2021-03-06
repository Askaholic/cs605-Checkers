// asknet.cpp
// Rohan Weeden
// Created: Feb. 2, 2018

// Implementation for AskNet neural network

#include "asknet3.h"
#include "immintrin.h"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <random>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <vector>


void * aligned_alloc(size_t size, float*& unaligned_storage) {
    const size_t align_size = 32;
    size_t request_size = size + align_size;
    const size_t needed = request_size;

    float * alloc = new float[needed];
    void * alloc_void = (void *) alloc;
    unaligned_storage = alloc;
    void * ptr = (float *) std::align(
        align_size,
        size,
        alloc_void,
        request_size
    );
    return ptr;
}

Network3::Network3(const std::vector<size_t> &topology){
    auto num_layers = topology.size();
    _layers = std::vector<Layer3>();

    size_t prev_num_nodes = 1;
    for (size_t i = 0; i < num_layers; i++) {
        auto num_nodes = topology[i];
        std::vector<float> weights(prev_num_nodes, 0.1f);
        auto node = Node3(weights);
        std::vector<Node3> nodes;
        for (size_t i = 0; i < num_nodes; i++) {
            nodes.emplace_back(weights);
        }

        _layers.emplace_back(nodes);
        prev_num_nodes = num_nodes;
    }
}

float Network3::evaluate(const std::vector<float> &inputs) {
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

void Network3::randomizeWeights() {
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

void Network3::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
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

std::vector<std::vector<std::vector<float>>> Network3::getWeights() {
    std::vector<std::vector<std::vector<float>>> out;
    for (size_t i = 0; i < _layers.size(); i++) {
        std::vector<std::vector<float>> layer_weights;
        for (size_t j = 0; j < _layers[i].size(); j++) {
            auto node = &_layers[i].getNodes()[j];
            std::vector<float> node_weights;
            for (size_t k = 0; k < node->size(); k++) {
                node_weights.push_back(node->getWeight(k));
            }
            layer_weights.push_back(node_weights);
        }
        out.push_back(layer_weights);
    }
    return out;
}

size_t Network3::getNumNodes() {
    size_t numNodes = 0;
    for (size_t i = 0; i < _layers.size(); i++) {
        numNodes += _layers[i].size();
    }
    return numNodes;
}

std::vector<float> Layer3::evaluate(const std::vector<float> &inputs) {
    std::vector<float> outputs;
    float * unaligned_temp;
    float * inputs_aligned = (float *) aligned_alloc(inputs.size(), unaligned_temp);
    std::copy(inputs.begin(), inputs.end(), inputs_aligned);
    for (size_t i = 0; i < _nodes.size(); i++) {
        outputs.push_back(_nodes[i].evaluate(inputs_aligned, inputs.size()));
    }
    delete[] unaligned_temp;
    return outputs;
}

std::vector<float> Layer3::evaluateFirst(const std::vector<float> &inputs) {
    auto layer_size = _nodes.size();
    if (inputs.size() != layer_size) {
        throw std::out_of_range("Must pass exactly " + std::to_string(layer_size) + " weights to Layer");
    }

    std::vector<float> output(layer_size);
    float * unaligned_temp;
    float * inputs_aligned = (float *) aligned_alloc(1, unaligned_temp);
    for (size_t i = 0; i < layer_size; i++) {
        inputs_aligned[0] = inputs[i];
        output[i] = _nodes[i].evaluate(inputs_aligned, 1);
    }
    delete[] unaligned_temp;
    return output;
}

Node3::Node3(const std::vector<float> & weights) {
    _size = weights.size();
    _array_size = _size + (_size % 16);
    _allocate(_array_size);

    std::copy(weights.begin(), weights.end(), _weights);
}

Node3::Node3(const Node3 & other) {
    _size = other._size;
    _array_size = other._array_size;
    _allocate(_array_size);

    std::copy(other._weights, other._weights + _size, _weights);
}

Node3::Node3(Node3 && other) {
    _size = 0;
    _array_size = 0;
    _weights = nullptr;
    _output_temp = nullptr;

    *this = std::move(other);
}

Node3 & Node3::operator=(const Node3 & other) {
    if (this == &other) {
        return *this;
    }
    _del();

    _size = other._size;
    _array_size = other._array_size;
    _allocate(_array_size);

    std::copy(other._weights, other._weights + _size, _weights);
    std::copy(other._output_temp, other._output_temp + _size, _output_temp);

    return *this;
}

Node3 & Node3::operator=(Node3 && other) {
    if (this == &other) {
        return *this;
    }
    _del();

    _size = other._size;
    _array_size = other._array_size;
    _weights = other._weights;
    _output_temp = other._output_temp;

    other._weights = nullptr;
    other._output_temp = nullptr;
    other._size = 0;
    other._array_size = 0;

    return *this;
}

Node3::~Node3() {
    _del();
}

void Node3::_allocate(size_t size) {
    float * w_unaligned;
    _weights = (float *) aligned_alloc(size, w_unaligned);
    _weights_unaligned = w_unaligned;

    float * o_unaligned;
    _output_temp = (float *) aligned_alloc(size, o_unaligned);
    _output_temp_unaligned = o_unaligned;

    if (_weights == NULL || _output_temp == NULL) {
        std::cout << "ERROR Got a nullptr when trying to allocate mem" << '\n';
    }
}

void Node3::_del() {
    if (_weights != nullptr) {
        delete[] _weights_unaligned;
    }
    if (_output_temp != nullptr) {
        delete[] _output_temp_unaligned;
    }
}

float Node3::evaluate(const float* inputs, size_t size) {
    if (size != _size) {
        throw std::out_of_range("Must pass exactly " + std::to_string(_size) + " weights to Node");
    }

    float sum = _sumWeights(inputs, size);
    return _applySigmoid(sum);
}

float Node3::_sumWeights(const float * inputs, size_t size) {
    float output = 0.0f;

    for (size_t i = 0; i < size; i+=8) {
        __m256 sse_in = _mm256_load_ps(&inputs[i]);
        __m256 sse_wt = _mm256_load_ps(&_weights[i]);
        __m256 sse_out = _mm256_mul_ps(sse_in, sse_wt);
        _mm256_store_ps(&_output_temp[i], sse_out);
    }
    // TODO: Horizontal add
    for (size_t i = 0; i < size; i++) {
        output += _output_temp[i];
    }
    return output;
}

float Node3::_applySigmoid(float num) {
    return std::tanh(num);
}
