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
