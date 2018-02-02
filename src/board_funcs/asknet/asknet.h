// asknet.h
// Rohan Weeden
// Created: Feb. 2, 2018

// Function definitions for AskNet neural network implementation


#ifndef ASK_NET_H
#define ASK_NET_H

#import <vector>
#import <cstddef>

class Network {
private:
    std::vector<Layer> _layers;

public:
    Network (std::vector<Layer> layers):_layers(layers) {};
};

class Layer {
private:
    std::vector<float> _weights;

public:
    Layer (size_t num_nodes);
};


#endif
