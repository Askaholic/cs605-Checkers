// test.cpp
// Rohan Weeden
// Created: Feb. 21, 2018

// Unit tests using the catch.hpp framework. Make sure you have catch.hpp in
// your path when compiling this file

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "board_funcs.h"
#include <memory>
#include <vector>


std::unique_ptr<BoardState> make_empty_board() {
    auto ptr = std::make_unique<BoardState>();
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        ptr->set(i, BLANK);
    }
    return ptr;
}

std::unique_ptr<BoardState> make_board(const std::vector<char> & board) {
    auto ptr = std::make_unique<BoardState>();
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        char value = BLANK;
        switch (board[i]) {
            case 'r': value = RED_CHECKER; break;
            case 'b': value = BLACK_CHECKER; break;
            case 'R': value = RED_KING; break;
            case 'B': value = BLACK_KING; break;
        }
        ptr->set(i, value);
    }
    return ptr;
}

bool operator==(const Jump & lhs, const Jump & rhs) {
    if (lhs._from != rhs._from) {
        return false;
    }
    if (lhs._to != rhs._to) {
        return false;
    }
    if (lhs._enemy != rhs._enemy) {
        return false;
    }

    return true;
}

TEST_CASE( "Generate Jumps", "[jump gen]") {
    JumpGenerator gen;

    SECTION( "Single jump" ) {
        auto board = make_board(
            {
                  'r',' ',' ',' ',
                ' ','b',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }
        );

        std::vector<Jump> correct_jumps = {
            {0, 9, 5}
        };

        auto moves = gen.get_possible_jumps(*board, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
}
