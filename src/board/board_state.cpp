// board_state.cpp
// Rohan Weeden
// Created: Jan. 31, 2018

// Implementation of BoardState data structures

#include "board_state.h"
#include <stdexcept>
#include <string>
#include <typeinfo>


BoardState::BoardState(const BoardState & board) {
    // if (&board == nullptr || &board == 0) {
    //     std::cout << "trying to copy nullptr...." << '\n';
    //     return;
    // }
    // if ((unsigned long)this >= 0x7fffffffcc00) {
        // std::cout << "board: " << &board << '\n';
    // }
    // for (size_t i = 0; i < 16; i++) {
    //     _tiles[i] = board._tiles[i];
    // }
    std::copy(board._tiles, board._tiles + (BOARD_ELEMENTS / 2), _tiles);
}

BoardState & BoardState::operator=(const BoardState & other) {
    if(this == &other) {
        return *this;
    }

    std::copy(other._tiles, other._tiles + (BOARD_ELEMENTS / 2), _tiles);
    return *this;
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

bool BoardState::operator==(const BoardState & other) const {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        if (other[i] != (*this)[i]) {
            return false;
        }
    }
    return true;
}

std::string BoardState::toStdString() const {
    std::string str = "";
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        auto piece = (*this)[i];
        char next = '.';
        switch (piece) {
            case RED_CHECKER: next='r'; break;
            case BLACK_CHECKER: next='b'; break;
            case RED_KING: next='R'; break;
            case BLACK_KING: next='B'; break;
            case BLANK: next='.'; break;
            default: next='?'; break;
        }
        str += next;
    }
    str += '\n';
    return str;
}

void BoardState::apply_move(const Move move) {
    auto piece = (*this)[move._from];
    set(move._from, BLANK);
    set(move._to, piece);
    make_king(move._to);
}

void BoardState::apply_jump(const Jump jump) {
    auto piece = (*this)[jump._from];
    set(jump._from, BLANK);
    set(jump._to, piece);
    set(jump._enemy, BLANK);
    make_king(jump._to);
}

void BoardState::make_king(const int to) {
    auto piece = (*this)[to];
    if (in_<int>(red_king_end, 4, to) && piece == RED_CHECKER) {
        set(to, RED_KING);
    }
    if (in_<int>(black_king_end, 4, to) && piece == BLACK_CHECKER) {
        set(to, BLACK_KING);
    }
}

bool BoardState::is_empty() {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        auto piece = (*this)[i];
        if (piece != BLANK) return false;
    }
    return true;
}


char BoardStateFast::operator[](size_t i) {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  return _tiles[i];
}

void BoardStateFast::set(size_t i, char val) {
  if (i < 0 || i > BOARD_ELEMENTS - 1) { throw std::out_of_range("Board index out of range " + std::to_string(i)); }
  _tiles[i] = val;
}
