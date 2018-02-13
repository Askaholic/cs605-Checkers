// asknet.h
// Rohan Weeden
// Created: Feb. 2, 2018

// Function definitions for AskNet neural network implementation


#ifndef ASK_NET3_H
#define ASK_NET3_H

#include <vector>
#include <cstddef>

class Node3 {
private:
    float * _weights;
    float * _output_temp;
    size_t _size;
    size_t _array_size;
    float _sumWeights(const std::vector<float> &inputs);
    float _applySigmoid(float num);
    void _del();
    void _allocate(size_t size);

public:
    Node3() = delete;
    Node3(const std::vector<float> & weights);
    Node3(const Node3 &);
    Node3(Node3 &&);
    Node3 & operator=(const Node3 &);
    Node3 & operator=(Node3 &&);
    ~Node3();
    float evaluate(const std::vector<float> &inputs);
    size_t size() { return _size; }
    void setWeight(size_t index, float weight) { _weights[index] = weight; }
    float getWeight(size_t index) { return _weights[index]; }
};

class Layer3 {
private:
    std::vector<Node3> _nodes;

public:
    Layer3 (std::vector<Node3> nodes):_nodes(nodes) {};
    std::vector<float> evaluate(const std::vector<float> &inputs);
    std::vector<float> evaluateFirst(const std::vector<float> &inputs);
    size_t size() { return _nodes.size(); }
    std::vector<Node3> & getNodes() { return _nodes; }
};

class Network3 {
private:
    std::vector<Layer3> _layers;

public:
    Network3 (std::vector<Layer3> layers):_layers(layers) {};
    Network3 (const std::vector<size_t> &topology);
    float evaluate(const std::vector<float> &inputs);
    void randomizeWeights();
    void setWeights(const std::vector<std::vector<std::vector<float>>> &);
    std::vector<std::vector<std::vector<float>>> getWeights();
    std::vector<Network3> & evolveWeights(size_t num_children, float stdev);
    size_t getNumNodes();
};


#endif
