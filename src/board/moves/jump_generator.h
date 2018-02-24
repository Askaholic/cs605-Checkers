// jump_generator.h
// Rohan Weeden
// Created: Feb. 24, 2018

// Header definitions for jump generator


#ifndef CHECKERS_JUMP_GENERATOR_H
#define CHECKERS_JUMP_GENERATOR_H


#include "board_state.h"
#include <cstddef>
#include <vector>


class JumpGenerator {
private:
    /* data */
    char _checker;
    char _king;
    int _player;
    void _append_jumps_for_piece(std::vector<BoardState> & jumps, const BoardState & board, size_t index);
    void _do_multi_jump(std::vector<BoardState> & jumps, const BoardState & board, size_t index, std::vector<Jump> & table_jumps);
    bool _is_players_piece(char piece);


public:
    JumpGenerator () {}

    std::vector<BoardState> get_possible_jumps(const BoardState & board, int player);

    virtual ~JumpGenerator () {}
};

#endif
