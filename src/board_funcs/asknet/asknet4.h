// asknet4.h
// Rohan Weeden
// Created: Feb. 13, 2018

// The 4th iteration of AskNet. This iteration aims to Allocate all of the
// Memory for the entire network in one contiguous chunk, for increased
// cache performance.

#ifndef ASK_NET4_h
#define ASK_NET4_h

#include <vector>
#include <cstddef>


/* Size of the header block for each Layer (in bytes) */
#define LAYER_HEADER_SIZE 4;


/*
 * Network class.
*/
class Network4 {
private:
    std::vector<float> _data;
    size_t _getRequiredSpace(const std::vector<size_t> & topology);
    size_t _getLayerRequiredSpace(size_t num_nodes, size_t num_node_weights);
    size_t _getNodeRequiredSpace(size_t num_weights);
    float * _writeLayerHeader(float * start, size_t num_nodes, size_t num_node_weights);

public:
    Network4 (const std::vector<size_t> & topology);
    float evaluate(std::vector<float> inputs);
    const std::vector<float> & getData() { return _data; }
};

#endif
