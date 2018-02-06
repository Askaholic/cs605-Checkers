// asknet.cpp
// Rohan Weeden
// Created: Feb. 2, 2018

// Implementation for AskNet neural network

#include "asknet.h"
#include <vector>
#include <cstddef>
#include <string>
#include <stdexcept>
#include <memory>

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
    if (num > 0.5f) { return 1.0f; }
    return 0.0f;
}
