// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H

#define BLANK 0x0
#define BLACK_CHECKER 0x1
#define RED_CHECKER 0x2
#define BLACK_KING 0x3
#define RED_KING 0x4

class Board {
public:
    const char& operator[](size_t);
private:
    char _tiles[32];
};

// Test function. To verify that the module is installed and works
char * test(char * str);

void setup_board();
Board get_board();


#endif
