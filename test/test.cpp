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
#include <string>
#include <stdexcept>


std::unique_ptr<BoardState> make_empty_board() {
    auto ptr = std::make_unique<BoardState>();
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        ptr->set(i, BLANK);
    }
    return ptr;
}

std::unique_ptr<BoardState> make_board(const std::vector<char> & board) {
    if (board.size() != BOARD_ELEMENTS) {
        throw std::out_of_range("Wrong number of board elements");
    }
    auto ptr = std::make_unique<BoardState>();
    for (size_t i = 0; i < board.size(); i++) {
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


bool operator==(const BoardState & lhs, const BoardState & rhs) {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

TEST_CASE( "Generate Jumps", "[jump gen]") {
    JumpGenerator gen;


    const auto board_1 = make_board(
        {
              'r',' ',' ',' ',    //0 - 3
            ' ','b',' ',' ',    //4 - 7
              ' ',' ',' ',' ',    //8 - 11
            ' ',' ',' ',' ',    //12 - 15
              ' ',' ',' ',' ',    //16 - 19
            ' ',' ',' ',' ',    //20 - 23
              ' ',' ',' ',' ',    //24 - 27
            ' ',' ',' ',' ',    //28 - 31
        }
    );

    const auto board_2 = make_board(
        {
              ' ',' ',' ',' ',    //0 - 3
            ' ',' ',' ',' ',    //4 - 7
              ' ','r',' ',' ',    //8 - 11
            ' ',' ','b',' ',    //12 - 15
              ' ',' ',' ',' ',    //16 - 19
            ' ',' ','b',' ',    //20 - 23
              ' ',' ',' ',' ',    //24 - 27
            ' ',' ',' ',' ',    //28 - 31
        }
    );

    SECTION( "Single jump - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',    //0 - 3
                ' ',' ',' ',' ',    //4 - 7
                  ' ','r',' ',' ',    //8 - 11
                ' ',' ',' ',' ',    //12 - 15
                  ' ',' ',' ',' ',    //16 - 19
                ' ',' ',' ',' ',    //20 - 23
                  ' ',' ',' ',' ',    //24 - 27
                ' ',' ',' ',' ',    //28 - 31
            }))
        };

        auto moves = gen.get_possible_jumps(*board_1, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
    SECTION( "Single jump - Black player" ) {
        std::vector<BoardState> correct_jumps = { };

        auto moves = gen.get_possible_jumps(*board_1, BLACK_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
    SECTION( "Double jump - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',    //0 - 3
                ' ',' ',' ',' ',    //4 - 7
                  ' ',' ',' ',' ',    //8 - 11
                ' ',' ',' ',' ',    //12 - 15
                  ' ',' ',' ',' ',    //16 - 19
                ' ',' ',' ',' ',    //20 - 23
                  ' ','r',' ',' ',    //24 - 27
                ' ',' ',' ',' ',    //28 - 31
            }))
        };

        auto moves = gen.get_possible_jumps(*board_2, BLACK_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
}
