// board_state.cpp
// Rohan Weeden
// Created: Jan. 31, 2018

// Implementation of BoardState data structures

#include <stdexcept>
#include <string>
#include "board_funcs.h"

BoardState::BoardState(const BoardState & board) {
    for (size_t i = 0; i < BOARD_ELEMENTS / 2; i++) {
        _tiles[i] = board._tiles[i];
    }
}

char BoardState::operator[](size_t i) {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  if (i & 0x1) {
    return _tiles[i / 2] & 0x0F;
  }
  return (_tiles[i / 2] >> 4) & 0x0F;
}

const char BoardState::operator[](size_t i) const {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  if (i & 0x1) {
    return _tiles[i / 2] & 0x0F;
  }
  return (_tiles[i / 2] >> 4) & 0x0F;
}


void BoardState::set(size_t i, char val) {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  if (i & 0x1) {
    _tiles[i / 2] = (_tiles[i / 2] & 0xF0) | (val & 0x0F);
  }
  else {
    _tiles[i / 2] = (_tiles[i / 2] & 0x0F) | ((val & 0x0F) << 4);
  }
}

void BoardState::apply_move(const Move move) {
    auto piece = (*this)[move._from];
    set(move._from, BLANK);
    set(move._to, piece);
}

char BoardStateFast::operator[](size_t i) {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  return _tiles[i];
}

void BoardStateFast::set(size_t i, char val) {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  _tiles[i] = val;
}

/* *************************************************** *
    Char * manipulation functions
 * *************************************************** */

char board_get_one(const char * start, size_t i) {
    // Remove the check eventually

    if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
    if (i & 0x1) {
        return start[i / 2] & 0x0F;
    }
    return (start[i / 2] >> 4) & 0x0F;
}

void board_set_one(char * start, size_t i, char val) {
    // Remove the check eventually
    if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
    if (i & 0x1) {
        start[i / 2] = (start[i / 2] & 0xF0) | (val & 0x0F);
    }
    else {
        start[i / 2] = (start[i / 2] & 0x0F) | ((val & 0x0F) << 4);
    }
}

void board_write(char * start, const BoardState & board) {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        board_set_one(start, i, board[i]);
    }
}

void board_read(const char * start, BoardState & board) {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        board.set(i, board_get_one(start, i));
    }
}
