// board_fincs.cpp
// Rohan Weeden
// Created: Jan, 26. 2018

// Shared library for code that needs to be fast and small...

#include <string>
#include <string.h>
#include "board_funcs.h"

char * test(char * str) {
    std::string in(str);
    auto result_str = std::string(str) + " HELLO WORLD";

    char * result = new char[result_str.length()];
    strcpy(result, result_str.c_str());

    return result;
}
