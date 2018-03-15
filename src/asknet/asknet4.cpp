// asknet3.cpp
// Rohan Weeden
// Created: Feb. 13, 2018

// Implementation for AskNet v4. Check the header file for documentation

#include "asknet4.h"
#include "aligned_array.h"
#include "immintrin.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>

size_t padSizeToAlignment(size_t size, size_t alignment) {
    auto overflow = size % alignment;
    if (overflow == 0) {
        return size;
    }
    return size + alignment - (overflow);
}


Network4::Network4(const std::vector<size_t> & topology) {
    size_t required_space = _getRequiredSpace(topology);
    // std::cout << "required space " << required_space << '\n';
    _num_layers = topology.size();

    if (_num_layers == 0) {
        return;
    }

    _data = AlignedArray<float, 32>(required_space);
    auto layer_start = _writeNetworkHeader(&_data[0], topology[0]);

    size_t num_node_inputs = 1;
    size_t numSigmas = 0;
    for (size_t i = 0; i < _num_layers; i++) {
        auto num_nodes_in_layer = topology[i];

        layer_start = _writeLayerHeader(layer_start, num_nodes_in_layer, num_node_inputs);
        num_node_inputs = num_nodes_in_layer;
        numSigmas += (topology[i] * num_node_inputs);
    }
    _sigmas = std::vector<float>(numSigmas);
}

float * Network4::_writeLayerHeader(float * start, size_t num_nodes, size_t num_node_weights) {
    size_t node_size = _getNodeRequiredSpace(num_node_weights);
    size_t total_size = _getLayerRequiredSpace(num_nodes, num_node_weights);

    start[0] = LAYER_HEADER_SIZE;
    start[1] = total_size;
    start[2] = num_nodes;
    start[3] = node_size;
    start[4] = num_node_weights;
    start[5] = LAYER_HEADER_SIZE + (num_nodes * node_size);

    float * end = start + total_size;
    return end;
}

float * Network4::_writeNetworkHeader(float * start, size_t num_inputs) {
    size_t inputs_size = padSizeToAlignment(num_inputs, 32);

    start[0] = NETWORK_HEADER_SIZE;
    start[1] = num_inputs;
    start[2] = inputs_size;
    start[3] = NETWORK_HEADER_SIZE;

    return start + NETWORK_HEADER_SIZE + inputs_size;
}

size_t Network4::_getRequiredSpace(const std::vector<size_t> & topology) {
    size_t required_space = 0;
    if (topology.size() > 0) {
        required_space += topology[0];
    }

    size_t num_node_inputs = 1;
    for (size_t i = 0; i < topology.size(); i++) {
        auto num_nodes_in_layer = topology[i];
        auto space = _getLayerRequiredSpace(num_nodes_in_layer, num_node_inputs);
        required_space += space;

        num_node_inputs = num_nodes_in_layer;
    }

    return required_space;
}

size_t Network4::_getLayerRequiredSpace(size_t num_nodes, size_t num_node_weights) {
    size_t node_size = _getNodeRequiredSpace(num_node_weights);
    size_t layer_header_size = LAYER_HEADER_SIZE;
    return padSizeToAlignment(layer_header_size + (num_nodes * node_size) + num_nodes, 32);
}

size_t Network4::_getNodeRequiredSpace(size_t num_weights) {
    size_t node_size = num_weights + 1;

    return padSizeToAlignment(node_size, 32);
}

inline LayerHeader Network4::_readLayerHeader(float * start) {
    return { (size_t) start[0], (size_t) start[1], (size_t) start[2], (size_t) start[3] , (size_t) start[4] , (size_t) start[5]};
}

inline NetworkHeader Network4::_readNetworkHeader(float * start) {
    return { (size_t) start[0], (size_t) start[1], (size_t) start[2], (size_t) start[3] };
}

void Network4::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    size_t i = 0;
    while (layerStart < dataEnd) {
        if (! (i < weights.size())) {
            throw std::out_of_range("Wrong number of layers (" + std::to_string(weights.size()) + ") passed to setWeights" );
        }
        auto header = _readLayerHeader(layerStart);
        if (weights[i].size() != header.num_nodes) {
            throw std::out_of_range("Wrong number of nodes (" + std::to_string(weights[i].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + ")" );
        }
        for (size_t j = 0; j < header.num_nodes; j++) {
            if (weights[i][j].size() != header.num_node_weights) {
                throw std::out_of_range("Wrong number of weights (" + std::to_string(weights[i][j].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + "), Node (" + std::to_string(j) + ")" );
            }
            for (size_t k = 0; k < header.num_node_weights; k++) {
                layerStart[header.size + (header.node_size * j) + k] = weights[i][j][k];
            }
        }
        layerStart += header.layer_size;
        i++;
    }
}


std::vector<std::vector<std::vector<float>>> Network4::getWeights() {
    std::vector<std::vector<std::vector<float>>> weights;
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    size_t i = 0;
    while (layerStart < dataEnd) {
        if (! (i < weights.size())) {
            throw std::out_of_range("Wrong number of layers (" + std::to_string(weights.size()) + ") passed to setWeights" );
        }
        auto header = _readLayerHeader(layerStart);
        if (weights[i].size() != header.num_nodes) {
            throw std::out_of_range("Wrong number of nodes (" + std::to_string(weights[i].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + ")" );
        }
        std::vector<std::vector<float>> layer;
        for (size_t j = 0; j < header.num_nodes; j++) {
            if (weights[i][j].size() != header.num_node_weights) {
                throw std::out_of_range("Wrong number of weights (" + std::to_string(weights[i][j].size()) + ") passed to setWeights. Layer (" + std::to_string(i) + "), Node (" + std::to_string(j) + ")" );
            }
            std::vector<float> node;
            for (size_t k = 0; k < header.num_node_weights; k++) {
                node.push_back(layerStart[header.size + (header.node_size * j) + k]);
            }
            layer.push_back(node);
        }
        weights.push_back(layer);
        layerStart += header.layer_size;
        i++;
    }
    return weights;
}


size_t Network4::getNumNodes() {
    size_t total = 0;
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        total += header.num_nodes;
        layerStart += header.layer_size;
    }
    return total;
}

size_t Network4::getNumWeights() {
    size_t total = 0;

    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        total += header.num_nodes * header.num_node_weights;
        layerStart += header.layer_size;
    }
    return total;
}

void Network4::setInputs(const std::vector<float> inputs) {
    auto net_header = _readNetworkHeader(&_data[0]);

    if (net_header.num_inputs != inputs.size()) {
        throw std::out_of_range("Wrong number of inputs (" + std::to_string(inputs.size()) + ") passed to evaluate. Expecting " + std::to_string( net_header.num_inputs ));
    }

    float * net_inputs = &_data[0] + net_header.input_offset;
    std::copy(&inputs[0], &inputs[0] + inputs.size(), net_inputs);
}

// TODO: Refactor this......
float Network4::evaluate() {
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);

    float * layer_inputs = layerStart + net_header.input_offset;

    layerStart += net_header.size + net_header.input_block_size;

    // TODO: Refactor first time special case
    auto header = _readLayerHeader(layerStart);
    float * layer_outputs = layerStart + header.output_offset;

    for (size_t i = 0; i < net_header.num_inputs; i++) {
        float output = layerStart[header.size + (header.node_size * i)] * layer_inputs[i];
        output = _applySigmoid(output);
        layer_outputs[i] = output;
    }
    layerStart += header.layer_size;

    while (layerStart < dataEnd) {
        header = _readLayerHeader(layerStart);
        layer_inputs = layer_outputs;
        layer_outputs = layerStart + header.output_offset;

        _evaluateLayer(layerStart, header, layer_inputs, layer_outputs);
        layerStart += header.layer_size;
    }
    return layer_outputs[0];
}

inline float horizontal_add (__m256 a) {
    __m256 t1 = _mm256_hadd_ps(a, a);
    __m256 t2 = _mm256_hadd_ps(t1, t1);
    __m128 t3 = _mm256_extractf128_ps(t2, 1);
    __m128 t4 = _mm_add_ss(_mm256_castps256_ps128(t2), t3);
    return _mm_cvtss_f32(t4);
}

static int maskTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0},
    {-1, -1, 0, 0, 0, 0, 0, 0},
    {-1, -1, -1, 0, 0, 0, 0, 0},
    {-1, -1, -1, -1, 0, 0, 0, 0},
    {-1, -1, -1, -1, -1, 0, 0, 0},
    {-1, -1, -1, -1, -1, -1, 0, 0},
    {-1, -1, -1, -1, -1, -1, -1, 0}
};
inline __m256 remove_overflow(__m256 a, int amount) {
    __m256 mask_ = _mm256_load_ps((float *)&maskTable[amount]);
    return _mm256_and_ps(a, mask_);
}

inline void Network4::_evaluateLayer(float * layer_start, LayerHeader & header, const float * inputs, float * outputs) {
    size_t node_size = header.node_size;
    size_t num_weights = header.num_node_weights;

    for (size_t j = 0; j < header.num_nodes; j++) {
        float node_output = 0.0f;

        for (size_t k = 0; k < num_weights; k += 8) {
            auto layer_start_index = header.size + (node_size * j) + k;
            __m256 sse_in = _mm256_load_ps(&inputs[k]);
            __m256 sse_wt = _mm256_load_ps(&layer_start[layer_start_index]);
            __m256 sse_out = _mm256_mul_ps(sse_in, sse_wt);
            if(k + 8 > num_weights) {
                sse_out = remove_overflow(sse_out, num_weights - k);
            }
            node_output += horizontal_add(sse_out);
        }
        node_output = _applySigmoid(node_output);
        outputs[j] = node_output;
    }
}



inline float Network4::_applySigmoid(float num) {
    return tanh(num);
}




void Network4::writeToFile(std::string filename){

    std::ofstream nnf;
    nnf.open(filename);


    for(size_t ii = 0; ii < _data.size(); ii++){
        nnf << _data[ii] << " ";
    }

    for(size_t ii = 0; ii < _sigmas.size(); ii++){
        nnf << _sigmas[ii] << " ";
    }

    nnf << _kingVal << " ";
    nnf.close();
}



void Network4::readFromFile(std::string filename){

    std::fstream nnf;
    std::string line;
    nnf.open(filename);
    float readFloat;



    for(int ii = 0; ii < _data.size(); ii++){
        std::getline(nnf, line, ' ');
        std::istringstream iss(line);
        iss >> readFloat;
        _data[ii] = readFloat;
    }

    for(int ii = 0; ii < _sigmas.size(); ii++){
        std::getline(nnf, line, ' ');
        std::istringstream iss(line);
        iss >> readFloat;
        _sigmas[ii] = readFloat;
    }

    std::getline(nnf, line, ' ');
    std::istringstream is(line);
    is >> readFloat;
    _kingVal = readFloat;
    nnf.close();
}


void Network4::randomizeWeights() {

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dist(-0.2, 0.2);

    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    size_t i = 0;
    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        for (size_t j = 0; j < header.num_nodes; j++) {
            for (size_t k = 0; k < header.num_node_weights; k++) {
                layerStart[header.size + (header.node_size * j) + k] = dist(engine);
            }
        }
        layerStart += header.layer_size;
        i++;
    }

}

float Network4::computeTau(){
    return 1 / (sqrt( 2 * sqrt( _sigmas.size() ) ) );
}


void Network4::printWeights() {
    float * layerStart = &_data[0];
    std::cout << "Weights: " << '\n';
    for (float * i = layerStart; i < layerStart + 100; i++) {
        std::cout << i[0] << " ";
    }
    std::cout << "..." << '\n';
}

void Network4::evolveSigmas(){

    std::random_device rd;
    std::mt19937 engine(rd());
    std::normal_distribution<float> dist(0.0, 1.0);

    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    size_t i = 0;
    size_t iter = 0;
    float tau = computeTau();
    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        for (size_t j = 0; j < header.num_nodes; j++) {
            for (size_t k = 0; k < header.num_node_weights; k++) {
                iter = header.size + (header.node_size * j) + k;
                _sigmas[iter] = _sigmas[iter] * exp(tau * dist(engine));
            }
        }
        layerStart += header.layer_size;
        // TODO: Maybe delete this
        if (header.layer_size < 1) {
            throw std::out_of_range("Layer size: " + std::to_string(header.layer_size));
        }
        i++;
    }
}

void Network4::evolveWeights(){

    std::random_device rd;
    std::mt19937 engine(rd());
    std::normal_distribution<float> dist(0.0, 1.0);

    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    auto net_header = _readNetworkHeader(layerStart);
    layerStart += net_header.size + net_header.input_block_size;

    size_t i = 0;
    size_t iter = 0;
    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        for (size_t j = 0; j < header.num_nodes; j++) {
            for (size_t k = 0; k < header.num_node_weights; k++) {
                iter = header.size + (header.node_size * j) + k;
                _data[iter] = _data[iter] + _sigmas[iter] * dist(engine);
            }
        }
        layerStart += header.layer_size;
        i++;
    }
}


void Network4::evolveKing(){
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dist(-0.1, 0.1);
    _kingVal = _kingVal + dist(engine);
}

void Network4::evolve() {
    evolveKing();
    evolveSigmas();
    evolveWeights();
}
