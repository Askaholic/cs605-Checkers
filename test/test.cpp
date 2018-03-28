// test.cpp
// Rohan Weeden
// Created: Feb. 21, 2018

// Unit tests using the catch.hpp framework. Make sure you have catch.hpp in
// your path when compiling this file

#include "catch.hpp"
#include "jump_generator.h"
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>


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
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ','r',' ',' ',
            ' ',' ','b',' ',
              ' ',' ',' ',' ',
            ' ',' ','b',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
        }
    );

    const auto board_3 = make_board(
        {
              ' ',' ',' ',' ',
            ' ','b','b',' ',
              ' ','r',' ',' ',
            ' ',' ','b',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
        }
    );

    const auto board_4 = make_board(
        {
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ','r',
              ' ','b','b',' ',
            ' ',' ',' ',' ',
        }
    );

    const auto board_5 = make_board(
        {
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ',' ','r',' ',
              ' ','b','b',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
        }
    );

    SECTION( "Single jump - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
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
    SECTION( "Single jump 2 - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ','b','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ','r',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_jumps(*board_3, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Single jump king - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','b',' ',' ',
                ' ',' ','R',' ',
            }))
        };

        auto moves = gen.get_possible_jumps(*board_4, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
    SECTION( "Double jump - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_jumps(*board_2, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
    SECTION( "Two jumps - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ','b',' ',
                ' ','r',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','b',' ',' ',
                ' ',' ',' ','r',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_jumps(*board_5, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }
}

TEST_CASE( "Generate Moves", "[move gen]") {
    JumpGenerator gen;


    const auto board_1 = make_board(
        {
              ' ',' ',' ',' ',    //0 - 3
            ' ','r',' ',' ',    //4 - 7
              ' ','b',' ',' ',    //8 - 11
            ' ',' ',' ',' ',    //12 - 15
              ' ',' ',' ',' ',    //16 - 19
            ' ',' ',' ',' ',    //20 - 23
              ' ',' ',' ',' ',    //24 - 27
            ' ',' ',' ',' ',    //28 - 31
        }
    );

    const auto board_2 = make_board(
        {
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ','r',' ',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ',' ','b',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
        }
    );

    const auto board_3 = make_board(
        {
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
              ' ','r','r',' ',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            ' ','b','b',' ',
              ' ',' ',' ',' ',
            ' ',' ',' ',' ',
        }
    );

    const auto board_4 = make_board(
        {
              'r','r','r','r',
            'r','r','r','r',
              'r','r','r','r',
            ' ',' ',' ',' ',
              ' ',' ',' ',' ',
            'b','b','b','b',
              'b','b','b','b',
            'b','b','b','b',
        }
    );

    SECTION( "Single move - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  'r','b',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_moves(*board_1, RED_PLAYER);
        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Single move - Black player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ','r','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_moves(*board_1, BLACK_PLAYER);

        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Two moves - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ','r',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ','r',' ',
                  ' ',' ',' ',' ',
                ' ',' ','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_moves(*board_2, RED_PLAYER);

        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Two moves - Black player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r',' ',' ',
                ' ',' ',' ',' ',
                  ' ','b',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ','b',' ',
                ' ',' ',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_moves(*board_2, BLACK_PLAYER);

        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Multiple moves - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ','r',' ',
                ' ','r',' ',' ',
                  ' ',' ',' ',' ',
                ' ','b','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ',' ','r',' ',
                ' ',' ','r',' ',
                  ' ',' ',' ',' ',
                ' ','b','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r',' ',' ',
                ' ',' ','r',' ',
                  ' ',' ',' ',' ',
                ' ','b','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r',' ',' ',
                ' ',' ',' ','r',
                  ' ',' ',' ',' ',
                ' ','b','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_moves(*board_3, RED_PLAYER);

        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Multiple moves - Black player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r','r',' ',
                ' ',' ',' ',' ',
                  'b',' ',' ',' ',
                ' ',' ','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r','r',' ',
                ' ',' ',' ',' ',
                  ' ','b',' ',' ',
                ' ',' ','b',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r','r',' ',
                ' ',' ',' ',' ',
                  ' ','b',' ',' ',
                ' ','b',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            })),
            *(make_board({
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
                  ' ','r','r',' ',
                ' ',' ',' ',' ',
                  ' ',' ','b',' ',
                ' ','b',' ',' ',
                  ' ',' ',' ',' ',
                ' ',' ',' ',' ',
            }))
        };

        auto moves = gen.get_possible_moves(*board_3, BLACK_PLAYER);

        REQUIRE( moves == correct_jumps );
    }

    SECTION( "Starting board - Red player" ) {
        std::vector<BoardState> correct_jumps = {
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  ' ','r','r','r',
                'r',' ',' ',' ',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            })),
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  ' ','r','r','r',
                ' ','r',' ',' ',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            })),
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  'r',' ','r','r',
                ' ','r',' ',' ',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            })),
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  'r',' ','r','r',
                ' ',' ','r',' ',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            })),
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  'r','r',' ','r',
                ' ',' ','r',' ',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            })),
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  'r','r',' ','r',
                ' ',' ',' ','r',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            })),
            *(make_board({
                  'r','r','r','r',
                'r','r','r','r',
                  'r','r','r',' ',
                ' ',' ',' ','r',
                  ' ',' ',' ',' ',
                'b','b','b','b',
                  'b','b','b','b',
                'b','b','b','b',
            }))
        };

        auto moves = gen.get_possible_moves(*board_4, RED_PLAYER);

        REQUIRE( moves == correct_jumps );
    }
}
