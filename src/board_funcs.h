// board_funcs.h
// Rohan Weeden
// Created: Jan. 27, 2018

// Header for C++ fast board functions

#ifndef PY_BOARD_FUNCS_H
#define PY_BOARD_FUNCS_H


struct board {
    char tiles[32];
};

// Test function. To verify that the module is installed and works
char * test(char * str);

void setup_board();
board get_board();

#endif
