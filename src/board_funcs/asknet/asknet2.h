// asknet2.h
// Rohan Weeden
// Created: Feb. 10, 2018

// Function definitions for AskNet neural network implementation, but using
// hopefully faster CPU operaions


#ifndef ASK_NET2_H
#define ASK_NET2_H

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
    Node2(const Node2 & node);
    Node2(Node2 && node);
    Node2 & operator=(const Node2 & node);
    Node2 & operator=(Node2 && node);
    ~Node2();
    float evaluate(const std::vector<float> &inputs);
    size_t size() { return _size; }
    size_t array_size() { return _array_size; }
    void setWeight(size_t index, float weight) { _weights[index] = weight; }
    float getWeight(size_t index) { return _weights[index]; }
};


class Layer2 {
private:
    size_t _weight_block_size;
    float * _layer_mem;
    size_t _layer_mem_size;
    float * _weights;
    size_t _weights_size;
    float * _outputs;
    size_t _output_size;

public:
    Layer2() {}
    Layer2(std::vector<Node2> nodes);
    Layer2(const Layer2 & layer);
    Layer2(const Layer2 && layer);
    Layer2 & operator=(const Layer2 & layer);
    Layer2 & operator=(Layer2 && layer);
    void evaluate(const float * inputs, size_t input_size);
    float * getWeights() { return _weights; }
    size_t getWeightsSize() { return _weights_size; }
    size_t size() { return _weights_size / _weight_block_size; }
    size_t getBlockSize() { return _weight_block_size; }
    float * getOutputs() { return _outputs; }
    size_t getOutputSize() { return _output_size; }
    ~Layer2();
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
