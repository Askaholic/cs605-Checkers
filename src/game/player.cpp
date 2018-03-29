// player.cpp
// Rohan Weeden
// Created: March 28, 2018

// Implementation of various player classes.

#include "player.h"
#include "piece_count.h"
#include "search.h"

#include <iostream>

BoardState UnPlayer::takeMove(const BoardState & board) {
    return board;
}


BoardState RandomPlayer::takeMove(const BoardState & board) {
    auto jumps = gen.get_possible_jumps(board, _color_id);
    if (jumps.size() != 0) {
        return jumps[std::rand() % jumps.size()];
    }

    auto moves = gen.get_possible_moves(board, _color_id);
    BoardState next = board;

    if (moves.size() == 0) {
        return next;
    }

    return moves[std::rand() % moves.size()];
}


BoardState PieceCountPlayer::takeMove(const BoardState & board) {
    auto result = min_max_search_inplace(board, _color_id, 4, &piece_count);
    return result.first;
}


BoardState AIPlayer::takeMove(const BoardState & board) {
    auto result = min_max_search_inplace(board, _color_id, 4, std::bind(&AIPlayer::evaluate, this, std::placeholders::_1, std::placeholders::_2));
    std::cout << "Score: " << result.second << '\n';
    return result.first;
}


float AIPlayer::evaluate(const BoardState & board, int player) {
    std::vector<float> inputs(BOARD_ELEMENTS);
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        auto piece = board[i];
        float score = 0;
        switch (piece) {
            case RED_CHECKER: score=1; break;
            case BLACK_CHECKER: score=-1; break;
            case RED_KING: score = _net.getKingValue(); break;
            case BLACK_KING: score = _net.getKingValue() * -1; break;
            default: score=0; break;
        }
        if (player == BLACK_PLAYER) {
            score *= -1;
        }
        inputs[i] = score;
    }
    _net.setInputs(inputs);
    // _net.printWeights();
    auto result = _net.evaluate();
    return result;
}
