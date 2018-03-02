// asknet.cpp
// Rohan Weeden
// Created: Feb. 2, 2018

// Implementation for AskNet neural network

#include "asknet.h"
#include <cmath>
#include <cstddef>
#include <random>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

Network::Network(const std::vector<size_t> &topology){
    auto num_layers = topology.size();
    _layers = std::vector<Layer>();

    size_t prev_num_nodes = 1;
    for (size_t i = 0; i < num_layers; i++) {
        auto num_nodes = topology[i];
        std::vector<float> weights(prev_num_nodes, 0.0f);
        std::vector<Node> nodes(num_nodes, weights);

        _layers.push_back(Layer(nodes));
        prev_num_nodes = num_nodes;
    }
}

float Network::evaluate(const std::vector<float> &inputs) {
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

void Network::randomizeWeights() {

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

void Network::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
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

std::vector<std::vector<std::vector<float>>> Network::getWeights() {
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

size_t Network::getNumNodes() {
    size_t numNodes = 0;
    for (size_t i = 0; i < _layers.size(); i++) {
        numNodes += _layers[i].size();
    }
    return numNodes;
}

std::vector<float> Layer::evaluate(const std::vector<float> &inputs) {
    std::vector<float> outputs;
    for (size_t i = 0; i < _nodes.size(); i++) {
        outputs.push_back(_nodes[i].evaluate(inputs));
    }
    return outputs;
}

std::vector<float> Layer::evaluateFirst(const std::vector<float> &inputs) {
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

float Node::evaluate(const std::vector<float> &inputs) {
    if (inputs.size() != _weights.size()) {
        throw std::out_of_range("Must pass exactly " + std::to_string(_weights.size()) + " weights to Node");
    }

    float sum = _sumWeights(inputs);
    return _applySigmoid(sum);
}

float Node::_sumWeights(const std::vector<float> &inputs) {
    float output = 0.0f;
    for (size_t i = 0; i < _weights.size(); i++) {
        output += inputs[i] * _weights[i];
    }
    return output;
}

float Node::_applySigmoid(float num) {
    return std::tanh(num);
}
