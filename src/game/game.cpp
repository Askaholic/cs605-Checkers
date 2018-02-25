// game.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Implementation of Game object
// See game.h for descriptions


#include "game.h"


Game::Game(const Player & red, const Player & black):
    _redPlayer(red),
    _blackPlayer(black) {
    reset();
}


void Game::playGame() {
    reset();
    while (!_has_winner) {
        playTurn();
    }
}


void Game::playTurn() {
    if (_has_winner) {
        return;
    }

    const Player & player = _current_turn_player == RED_PLAYER ? _redPlayer : _blackPlayer;

    BoardState newBoard = player.takeMove(_board);

    if (_board == newBoard) {
        _has_winner = true;
    }

    _board = newBoard;
    _endTurn();
}


void Game::reset() {
    _turn_count = 0;
    _current_turn_player = RED_PLAYER;
    _has_winner = false;
    _setupStartingBoard();
}


void Game::_setupStartingBoard() {
    _board = BoardState();

    for (size_t i = 0; i < 12; i++) {
        _board.set(i, RED_CHECKER);
        _board.set(BOARD_ELEMENTS - 1 - i, BLACK_CHECKER);
    }
}

void Game::_endTurn() {
    _turn_count++;

    // Swap the current player
    _current_turn_player = _current_turn_player == RED_PLAYER ? BLACK_PLAYER : RED_PLAYER;
}
