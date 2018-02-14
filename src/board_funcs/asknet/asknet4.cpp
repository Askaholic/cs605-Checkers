// asknet3.cpp
// Rohan Weeden
// Created: Feb. 13, 2018

// Implementation for AskNet v4. Check the header file for documentation

#include "asknet4.h"
#include "aligned_array.h"
#include "immintrin.h"
#include <cmath>
#include <vector>
#include <cstddef>
#include <iostream>
#include <stdexcept>


Network4::Network4(const std::vector<size_t> & topology) {
    size_t required_space = _getRequiredSpace(topology);
    std::cout << "required space " << required_space << '\n';
    _num_layers = topology.size();

    _data = AlignedArray<float, 32>(required_space);

    auto layer_start = &_data[0];
    size_t num_node_inputs = 1;
    for (size_t i = 0; i < topology.size(); i++) {
        auto num_nodes_in_layer = topology[i];

        layer_start = _writeLayerHeader(layer_start, num_nodes_in_layer, num_node_inputs);
        num_node_inputs = num_nodes_in_layer;
    }
}

float * Network4::_writeLayerHeader(float * start, size_t num_nodes, size_t num_node_weights) {
    size_t node_size = _getNodeRequiredSpace(num_node_weights);
    size_t total_size = _getLayerRequiredSpace(num_nodes, num_node_weights);

    start[0] = LAYER_HEADER_SIZE;
    start[1] = num_nodes;
    start[2] = node_size;
    start[3] = num_node_weights;

    float * end = start + total_size;
    return end;
}

size_t Network4::_getRequiredSpace(const std::vector<size_t> & topology) {
    size_t required_space = 0;

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
    return layer_header_size + (num_nodes * node_size);
}

size_t Network4::_getNodeRequiredSpace(size_t num_weights) {
    auto r = num_weights % 32;
    if (r == 0) {
        return num_weights;
    }
    return num_weights + 32 - (r);
}

LayerHeader Network4::_readLayerHeader(float * start) {
    return { start[0], start[1], start[2], start[3] };
}

void Network4::setWeights(const std::vector<std::vector<std::vector<float>>> & weights) {
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

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
                layerStart[(size_t) header.size + ((size_t) header.node_size * j) + k] = weights[i][j][k];
            }
        }
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
        i++;
    }
}

std::vector<std::vector<std::vector<float>>> Network4::getWeights() {
    std::vector<std::vector<std::vector<float>>> weights;
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

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
                node.push_back(layerStart[(size_t) header.size + ((size_t) header.node_size * j) + k]);
            }
            layer.push_back(node);
        }
        weights.push_back(layer);
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
        i++;
    }
    return weights;
}


size_t Network4::getNumNodes() {
    size_t total = 0;
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        total += (size_t) header.num_nodes;
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
    }
    return total;
}

size_t Network4::getNumWeights() {
    size_t total;

    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    while (layerStart < dataEnd) {
        auto header = _readLayerHeader(layerStart);
        total += (size_t) header.num_nodes * (size_t) header.num_node_weights;
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
    }
    return total;
}

// TODO: Refactor this......
float Network4::evaluate(const std::vector<float> & inputs) {
    float * layerStart = &_data[0];
    float * dataEnd = &_data[_data.size() - 1];

    AlignedArray<float, 32> layer_outputs(inputs.size());

    auto header = _readLayerHeader(layerStart);
    if (header.num_nodes != layer_outputs.size()) {
        throw std::out_of_range("Wrong number of inputs (" + std::to_string(layer_outputs.size()) + ") passed to evaluate. Expecting " + std::to_string((size_t) header.num_nodes ));
    }
    for (size_t i = 0; i < inputs.size(); i++) {
        float output = layerStart[(size_t)header.size + ((size_t) header.node_size * i)] * inputs[i];
        output = _applySigmoid(output);
        layer_outputs[i] = output;
    }
    layerStart += (size_t) (header.size + header.num_nodes * header.node_size);

    size_t i = 1;
    while (layerStart < dataEnd) {
        // Does a copy
        // TODO: Use pointer to vector instead
        AlignedArray<float, 32> layer_inputs(layer_outputs);

        header = _readLayerHeader(layerStart);

        _evaluateLayer(layerStart, header, layer_inputs, layer_outputs);
        layerStart += (size_t) (header.size + header.num_nodes * header.node_size);
        i++;
    }
    return layer_outputs[0];
}

float horizontal_add (__m256 a) {
    __m256 t1 = _mm256_hadd_ps(a, a);
    __m256 t2 = _mm256_hadd_ps(t1, t1);
    __m128 t3 = _mm256_extractf128_ps(t2, 1);
    __m128 t4 = _mm_add_ss(_mm256_castps256_ps128(t2), t3);
    return _mm_cvtss_f32(t4);
}

// Slower than horizontal_add?
float avx_only_hadd(__m256 const& v) {
    auto x = _mm256_permute2f128_ps(v, v, 1);
    auto y = _mm256_add_ps(v, x);
    x = _mm256_shuffle_ps(y, y, _MM_SHUFFLE(2, 3, 0, 1));
    x = _mm256_add_ps(x, y);
    y = _mm256_shuffle_ps(x, x, _MM_SHUFFLE(1, 0, 3, 2));
    return _mm_cvtss_f32(
        _mm256_castps256_ps128(
            _mm256_add_ps(x, y)
        )
    );
}

void Network4::_evaluateLayer(float * layer_start, LayerHeader & header, const AlignedArray<float, 32> & inputs, AlignedArray<float, 32> & outputs) {
    auto inputs_size = inputs.size();
    size_t node_size = header.node_size;
    size_t num_weights = header.num_node_weights;

    if (header.num_node_weights != inputs_size) {
        throw std::out_of_range("Wrong number of inputs (" + std::to_string(inputs_size) + ") passed to evaluate. Expecting " + std::to_string((size_t) header.num_node_weights ));
    }
    outputs.resize((size_t) header.num_nodes);

    for (size_t j = 0; j < (size_t)header.num_nodes; j++) {
        float node_output = 0.0f;

        // #pragma omp parallel for reduction(+:node_output)
        for (size_t k = 0; k < num_weights; k+=8) {
            auto layer_start_index = (size_t) header.size + (node_size * j) + k;
            __m256 sse_in = _mm256_load_ps(&inputs[k]);
            __m256 sse_wt = _mm256_load_ps(&layer_start[layer_start_index]);
            __m256 sse_out = _mm256_mul_ps(sse_in, sse_wt);
            node_output += horizontal_add(sse_out);
        }
        node_output = _applySigmoid(node_output);
        outputs[j] = node_output;
    }
}


float Network4::_applySigmoid(float num) {
    return tanh(num);
}
