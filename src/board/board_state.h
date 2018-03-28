// board_state.h
// Rohan Weeden
// Created: Feb. 24, 2018

// Header definitions for board state objects


#ifndef CHECKERS_AI_BOARD_STATE_H
#define CHECKERS_AI_BOARD_STATE_H


#include <cstddef>
#include <string>



#define RED_PLAYER 0
#define BLACK_PLAYER 1


#define BLANK 0x00
#define BLACK_CHECKER 0x01
#define RED_CHECKER 0x02
#define BLACK_KING 0x03
#define RED_KING 0x04


#define BOARD_ELEMENTS 32


const int red_king_end[] = {28, 29, 30, 31};
const int black_king_end[] = {0, 1, 2, 3};


struct Move {
    size_t _from;
    size_t _to;
};

struct Jump {
    size_t _from;
    size_t _to;
    size_t _enemy;
};



class BoardState {
public:
    char _tiles[BOARD_ELEMENTS/2 ];

public:
    BoardState() = default;
    BoardState(const BoardState &);
    BoardState & operator=(const BoardState & other);
    bool operator==(const BoardState & other) const;
    std::string toStdString() const;
    char operator[](size_t index);
    const char operator[](size_t index) const;
    void set(size_t index, char value);
    // test timing with reference?
    void apply_move(const Move);
    void apply_jump(const Jump);
    void make_king(const int to);
    bool is_empty() const;

};

std::ostream & operator<<(std::ostream & os, const BoardState & board);


class BoardStateFast {
private:
    char _tiles[BOARD_ELEMENTS];

public:
    char operator[](size_t index);
    void set(size_t index, char value);
};


template <typename T>
bool in_(const T * array, size_t length, const T &value) {
    for (size_t i = 0; i < length; i++) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
};


#endif
