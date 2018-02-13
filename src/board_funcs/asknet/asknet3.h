// asknet.h
// Rohan Weeden
// Created: Feb. 2, 2018

// Function definitions for AskNet neural network implementation


#ifndef ASK_NET3_H
#define ASK_NET3_H

#include <vector>
#include <cstddef>

template <size_t N>
class Node3 {
private:
    float _weights[N] __attribute__ ((aligned (32)));
    float _output_temp[N] __attribute__ ((aligned (32)));
    float _sumWeights(const std::vector<float> &inputs);
    float _applySigmoid(float num);

public:
    Node3() = delete;
    Node3(const std::vector<float> & weights);
    float evaluate(const std::vector<float> &inputs);
    size_t size() { return N; }
    void setWeight(size_t index, float weight) { _weights[index] = weight; }
    float getWeight(size_t index) { return _weights[index]; }
};

template <size_t N>
class Layer3 {
private:
    std::vector<Node3<N>> _nodes;

public:
    Layer3 (std::vector<Node3<N>> nodes):_nodes(nodes) {};
    std::vector<float> evaluate(const std::vector<float> &inputs);
    std::vector<float> evaluateFirst(const std::vector<float> &inputs);
    size_t size() { return _nodes.size(); }
    std::vector<Node3<N>> & getNodes() { return _nodes; }
};

template <size_t N>
class Network3 {
private:
    std::vector<Layer3<N>> _layers;

public:
    Network3 (std::vector<Layer3<N>> layers):_layers(layers) {};
    Network3 (const std::vector<size_t> &topology);
    float evaluate(const std::vector<float> &inputs);
    void randomizeWeights();
    void setWeights(const std::vector<std::vector<std::vector<float>>> &);
    std::vector<std::vector<std::vector<float>>> getWeights();
    std::vector<Network3> & evolveWeights(size_t num_children, float stdev);
    size_t getNumNodes();
};


#endif
