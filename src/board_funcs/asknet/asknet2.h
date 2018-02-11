// asknet2.h
// Rohan Weeden
// Created: Feb. 10, 2018

// Function definitions for AskNet neural network implementation, but using
// hopefully faster CPU operaions


#ifndef ASK_NET_H
#define ASK_NET_H

#include <vector>
#include <cstddef>


class Node2 {
private:
    float * _weights __attribute__ ((aligned (16))) ;
    float * _output_temp __attribute__ ((aligned (16))) ;
    size_t _size;
    size_t _array_size;
    float _sumWeights(const float * inputs);
    float _applySigmoid(float num);

public:
    Node2(const std::vector<float> & weights);
    float evaluate(const std::vector<float> &inputs);
    size_t size() { return _size; }
    void setWeight(size_t index, float weight) { _weights[index] = weight; }
    float getWeight(size_t index) { return _weights[index]; }
    ~Node2();
};


class Layer2 {
private:
    std::vector<Node2> _nodes;

public:
    Layer2 (std::vector<Node2> nodes):_nodes(nodes) {};
    std::vector<float> evaluate(const std::vector<float> &inputs);
    std::vector<float> evaluateFirst(const std::vector<float> &inputs);
    size_t size() { return _nodes.size(); }
    std::vector<Node2> & getNodes() { return _nodes; }
};


class Network2 {
private:
    std::vector<Layer2> _layers;

public:
    Network2 (std::vector<Layer2> layers):_layers(layers) {};
    Network2 (const std::vector<size_t> &topology);
    float evaluate(const std::vector<float> &inputs);
    void randomizeWeights();
    void setWeights(const std::vector<std::vector<std::vector<float>>> &);
    std::vector<Network2> & evolveWeights(size_t num_children, float stdev);
    size_t getNumNodes();
};


#endif
