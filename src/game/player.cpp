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


BoardState RandomSearchPlayer::takeMove(const BoardState & board) {
    auto result = min_max_search(board, _color_id, _depth, std::bind(&RandomSearchPlayer::evaluate, this, std::placeholders::_1, std::placeholders::_2));
    return result.first;
}


float RandomSearchPlayer::evaluate(const BoardState & board, int player) {
    return 1 - 2 * (float) std::rand() / (float) RAND_MAX;
}


BoardState PieceCountPlayer::takeMove(const BoardState & board) {
    auto result = min_max_search_inplace(board, _color_id, _depth, &piece_count);
    return result.first;
}


BoardState AIPlayer::takeMove(const BoardState & board) {
    auto result = min_max_search(board, _color_id, _depth, std::bind(&AIPlayer::evaluate, this, std::placeholders::_1, std::placeholders::_2));
    // std::cout << "Score: " << result.second << '\n';
    return result.first;
}


float AIPlayer::evaluate(const BoardState & board, int player) {
    auto inputs = _boardStateToNetworkInputs(board, player);
    _net.setInputs(inputs);
    // _net.printWeights();
    auto result = _net.evaluate();
    return result;
}


std::vector<float> AIPlayer::_boardStateToNetworkInputs(const BoardState & board, int player) {
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
    return inputs;
}



BoardState AIPlayer3Net::takeMove(const BoardState & board) {
    auto result = min_max_search(board, _color_id, _depth,
        std::bind(&AIPlayer3Net::evaluate, this, std::placeholders::_1, std::placeholders::_2));
    return result.first;
}


float AIPlayer3Net::evaluate(const BoardState & board, int player) {
    int numPieces = 0;
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        if (board[i] != BLANK) {
            numPieces++;
        }
    }
    Network4 & net = _beg;
    if (numPieces < 9) {
        net = _end;
    }
    else if (numPieces < 17) {
        net = _mid;
    }
    return _evaluateWithNetwork(board, player, net);
}

float AIPlayer3Net::_evaluateWithNetwork(const BoardState & board, int player, Network4 & net) {
    std::vector<float> inputs(BOARD_ELEMENTS);
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        auto piece = board[i];
        float score = 0;
        switch (piece) {
            case RED_CHECKER: score=1; break;
            case BLACK_CHECKER: score=-1; break;
            case RED_KING: score = net.getKingValue(); break;
            case BLACK_KING: score = net.getKingValue() * -1; break;
            default: score=0; break;
        }
        if (player == BLACK_PLAYER) {
            score *= -1;
        }
        inputs[i] = score;
    }
    net.setInputs(inputs);
    // _net.printWeights();
    auto result = net.evaluate();
    return result;
}

float AIPlayer3NetWithPieceCount::_evaluateWithNetwork(const BoardState & board, int player, Network4 & net) {
    std::vector<float> inputs(BOARD_ELEMENTS);
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        auto piece = board[i];
        float score = 0;
        switch (piece) {
            case RED_CHECKER: score=1; break;
            case BLACK_CHECKER: score=-1; break;
            case RED_KING: score = net.getKingValue(); break;
            case BLACK_KING: score = net.getKingValue() * -1; break;
            default: score=0; break;
        }
        if (player == BLACK_PLAYER) {
            score *= -1;
        }
        inputs[i] = score;
    }
    inputs.push_back(piece_count(board, player));
    net.setInputs(inputs);
    // _net.printWeights();
    auto result = net.evaluate();
    return result;
}
