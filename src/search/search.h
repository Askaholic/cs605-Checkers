// search.h
// Rohan Weeden
// Created: Feb. 24, 2018

// Definitions for search functions


#ifndef CHECKERS_AI_SEARCH_H
#define CHECKERS_AI_SEARCH_H


#include "board_state.h"
#include <functional>


#define BRANCH_FACTOR 32


std::pair<BoardState, float> min_max_search_inplace(
    const BoardState & board,
    int player,
    int depth,
    std::function<float(const BoardState &, int)>
);


#endif
