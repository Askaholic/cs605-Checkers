// search.h
// Rohan Weeden
// Created: Feb. 24, 2018

// Definitions for search functions


#ifndef CHECKERS_AI_SEARCH_H
#define CHECKERS_AI_SEARCH_H


#include "board_state.h"
#include <functional>
#include <tuple>


#define BRANCH_FACTOR 32

std::pair<BoardState, float> min_max_search(
    const BoardState & board,
    int player,
    int depth,
    std::function<float(const BoardState &, int)> evaluate
);

std::pair<BoardState, float> min_max_search_inplace(
    const BoardState & board,
    int player,
    int depth,
    std::function<float(const BoardState &, int)> evaluate
);

std::tuple<BoardState, float, int> min_max_search_ids(
    const BoardState & board,
    int player,
    int start_depth,
    size_t check_interval,
    double max_seconds,
    std::function<float(const BoardState &, int)> evaluate
);

#endif
