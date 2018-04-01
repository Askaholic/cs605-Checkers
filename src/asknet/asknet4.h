// asknet4.h
// Rohan Weeden
// Created: Feb. 13, 2018

// The 4th iteration of AskNet. This iteration aims to Allocate all of the
// Memory for the entire network in one contiguous chunk, for increased
// cache performance.

#ifndef ASK_NET4_h
#define ASK_NET4_h

#include "aligned_array.h"
#include <cstddef>
#include <string>
#include <vector>


/* Size of the header block for each Layer (in bytes) */
#define LAYER_HEADER_SIZE 8
#define NETWORK_HEADER_SIZE 8


struct NetworkHeader {
    size_t size;
    size_t num_inputs;
    size_t input_block_size;
    size_t input_offset;
};

struct LayerHeader {
    size_t size;
    size_t layer_size;
    size_t num_nodes;
    size_t node_size;
    size_t num_node_weights;
    size_t output_offset;
};

/*
 * Network class.
*/
class Network4 {
private:
    float _kingVal = 1.4;
    AlignedArray<float, 32> _data;
    std::vector<float> _sigmas;
    float _piece_count_input;
    float _piece_count_weight;
    float _piece_count_sigma;

    float _result_weight;
    float _result_sigma;

    size_t _num_layers;
    size_t _getRequiredSpace(const std::vector<size_t> & topology);
    size_t _getLayerRequiredSpace(size_t num_nodes, size_t num_node_weights);
    size_t _getNodeRequiredSpace(size_t num_weights);
    float * _writeLayerHeader(float * start, size_t num_nodes, size_t num_node_weights);
    float * _writeNetworkHeader(float * start, size_t num_inputs);
    LayerHeader _readLayerHeader(float * start);
    NetworkHeader _readNetworkHeader(float * start);
    void _evaluateLayer(float * start, LayerHeader & header, const float * inputs, float * outputs);
    float _applySigmoid(float);
    void _initSigmas();

public:
    Network4 (const std::vector<size_t> & topology);
    void setWeights(const std::vector<std::vector<std::vector<float>>> & weights);
    std::vector<std::vector<std::vector<float>>> getWeights();
    void randomizeWeights();
    void setInputs(const std::vector<float> inputs);
    float evaluate();
    const AlignedArray<float, 32> & getData() { return _data; }
    size_t getNumNodes();
    size_t getNumWeights();
    float getKingValue() { return _kingVal; }

    void writeToFile(std::string filename) const;
    void readFromFile(std::string filename);
    float computeTau();
    void evolveKing();
    void evolveSigmas();
    void evolveWeights();
    void evolve();

    // DEBUG
    void printWeights();
};

#endif
