// game.cpp
// Rohan Weeden
// Created: Feb. 24, 2018

// Implementation of Game object
// See game.h for descriptions


#include "game.h"

Game::Game(Player & red, Player & black):
    _redPlayer(red),
    _blackPlayer(black) {
    reset();
}


int Game::getWinner() const {
    if (_turn_count >= _max_turn_count) {
        return -1;
    }
    return getCurrentPlayer();
}


void Game::playGame() {
    reset();
    while (!_has_winner && _turn_count < _max_turn_count) {
        playTurn();
    }
}


void Game::playTurn() {
    if (_has_winner) {
        return;
    }

    Player & player = _current_turn_player == RED_PLAYER ? _redPlayer : _blackPlayer;

    BoardState newBoard = player.takeMove(_board);

    if (newBoard.is_empty() || _board == newBoard) {
        _has_winner = true;
    }

    if (!newBoard.is_empty()) {
        _board = newBoard;
    }
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
