// move_table.cpp
// Rohan Weeden
// Created: March 27, 2018

// Definition of move table...

#include "move_table.h"


std::vector<std::vector<int>> moveTable = {
    {-1,-1,   4,5},
    {-1,-1,   5,6},
    {-1,-1,   6,7},
    {-1,-1,  -1,7},

    {-1,0,   -1,8},
    {0,1,     8,9},
    {1,2,     9,10},
    {2,3,     10,11},

    {4,5,    12,13},
    {5,6,    13,14},
    {6,7,    14,15},
    {7,-1,   15,-1},

    {-1,8,  -1,16},
    {8,9,    16,17},
    {9,10,   17,18},
    {10,11,  18,19},

    {12,13,  20,21},
    {13,14,  21,22},
    {14,15,  22,23},
    {15,-1,  23,-1},

    {-1,16,  -1,24},
    {16,17,  24,25},
    {17,18,  25,26},
    {18,19,  26,27},

    {20,21,  28,29},
    {21,22,  29,30},
    {22,23,  30,31},
    {23,-1,  31,-1},

    {-1,24,  -1,-1},
    {24,25,  -1,-1},
    {25,26,  -1,-1},
    {26,27,  -1,-1}
};
