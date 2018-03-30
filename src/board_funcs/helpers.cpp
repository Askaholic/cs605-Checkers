// helpers.cpp
// Rohan Weeden
// Created: Jan. 31, 2018

// Miscellaneous board_funcs helper function implementations

#include <chrono>
#include <iostream>
#include <string>
#include <string.h>
#include "board_funcs.h"


void time_boards() {
  BoardState b_slow = {};
  BoardStateFast b_fast = {};

  const size_t COUNT = 1000000000;
  auto start_slow = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < COUNT; i++) {
    char tmp = b_slow[i % 32];
    b_slow.set((i + (int)tmp) % 32, 0x03);
  }
  auto end_slow = std::chrono::high_resolution_clock::now();

  auto start_fast = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < COUNT; i++) {
    char tmp = b_fast[i % 32];
    b_fast.set((i + (int)tmp) % 32, 0x03);
  }
  auto end_fast = std::chrono::high_resolution_clock::now();

  auto slow_time = ((std::chrono::nanoseconds)(end_slow - start_slow)).count();
  auto fast_time = ((std::chrono::nanoseconds)(end_fast - start_fast)).count();
  std::cout << "Slow: " << ((double)slow_time / COUNT) << " / call\n";
  std::cout << "Fast: " << ((double)fast_time / COUNT) << " / call\n";

}

bool is_valid_index(const int index) {
    if (index < 0 || index > 31)
        return false;
    return true;
}

void print_board(const BoardState & board) {
    print_board(board, "");
}

void print_board(const BoardState & board, std::string linePrefix) {

    std::cout << linePrefix;

    bool odd = false;
    for (size_t i = 0; i < 32; i++) {
        auto piece = board[i];
        char next = '.';
        switch (piece) {
            case RED_CHECKER: next='r'; break;
            case BLACK_CHECKER: next='b'; break;
            case RED_KING: next='R'; break;
            case BLACK_KING: next='B'; break;
            default: next='.'; break;
        }
        if (i % 4 == 0 and i != 0) {
            odd = !odd;
            std::cout << '\n' << linePrefix;
        }
        if (!odd) {
            std::cout << " " << next;
        }
        else {
            std::cout << next << " ";
        }
    }
    std::cout << '\n';
}
