// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include <stdexcept>
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
Board the_board = {};

char Board::operator[](size_t i) {
  if (i < 0 || i > 31) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  if (i & 0x1) {
    return _tiles[i / 2] & 0x0F;
  }
  return (_tiles[i / 2] >> 4) & 0x0F;
}

void Board::set(size_t i, char val) {
  if (i < 0 || i > 31) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  if (i & 0x1) {
    _tiles[i / 2] = (_tiles[i / 2] & 0xF0) | (val & 0x0F);
  }
  else {
    _tiles[i / 2] = (_tiles[i / 2] & 0x0F) | ((val & 0x0F) << 4);
  }
}

char * test(char * str) {
    std::string in(str);
    auto result_str = std::string(str) + " " + std::to_string(sizeof(Board));

    char * result = new char[result_str.length()];
    strcpy(result, result_str.c_str());

    return result;
}

void setup_board() {
    for (size_t i = 0; i < 12; i++) {
        the_board.set(i, RED_CHECKER);
        the_board.set(31 - i, BLACK_CHECKER);
    }
}

Board get_board() {
  return the_board;
}

std::vector<Board> get_possible_moves(Board board, int player) {

}
