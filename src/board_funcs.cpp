// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include <string>
#include <string.h>
#include <vector>
#include <cstddef>
#include "board_funcs.h"

std::vector<std::vector<int>> moveTable = {
    {-1, -1,		-1, 4},
    {-1, -1,		4, 5},
    {-1, -1,		5, 6},
    {-1, -1,		6, 7},

    {0, 1,		8, 9},
    {1, 2,		9, 10},
    {2, 3,		10, 11},
    {3, -1,		11, -1},

    {-1, 4,		-1, 12},
    {4, 5,		12, 13},
    {5, 6,		13, 14},
    {6, 7,		14, 15},

    {8, 9,		16, 17},
    {9, 10,		17, 18},
    {10, 11,		18, 19},
    {11, -1,		19, -1},

    {-1, 12,		-1, 20},
    {12, 13,		20, 21},
    {13, 14,		21, 22},
    {14, 15,		22, 23},

    {16, 17,		24, 25},
    {17, 18,		25, 26},
    {18, 19,		26, 27},
    {19, -1,		27, -1},

    {-1, 20,		-1, 28},
    {20, 21,		28, 29},
    {21, 22,		29, 30},
    {22, 23,		30, 31},

    {24, 25,		-1, -1},
    {25, 26,		-1, -1},
    {26, 27, 	-1, -1},
    {27, -1,		-1, -1}
};

board the_board = {};


char * test(char * str) {
    std::string in(str);
    auto result_str = std::string(str) + " " + std::to_string(sizeof(board));

    char * result = new char[result_str.length()];
    strcpy(result, result_str.c_str());

    return result;
}

void setup_board() {
    for (size_t i = 0; i < 12; i++) {
        the_board.tiles[i] = 'r';
        the_board.tiles[31 - i] = 'b';
    }
}

board get_board() {
  return the_board;
}
