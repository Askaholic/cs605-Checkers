// asknet.h
// Rohan Weeden
// Created: Feb. 2, 2018

// Function definitions for AskNet neural network implementation


#ifndef ASK_NET_H
#define ASK_NET_H

#include <vector>
#include <cstddef>


class Node {
private:
    std::vector<float> _weights;
    float _sumWeights(const std::vector<float> &inputs);
    float _applySigmoid(float num);

public:
    Node(std::vector<float> weights):_weights(weights) {};
    float evaluate(const std::vector<float> &inputs);
};


class Layer {
private:
    std::vector<Node> _nodes;

public:
    Layer (std::vector<Node> nodes):_nodes(nodes) {};
    std::vector<float> evaluate(const std::vector<float> &inputs);
    std::vector<float> evaluateFirst(const std::vector<float> &inputs);
};


class Network {
private:
    std::vector<Layer> _layers;

public:
    Network (std::vector<Layer> layers):_layers(layers) {};
    Network (const std::vector<size_t> &topology);
    float evaluate(const std::vector<float> &inputs);
};






#endif
