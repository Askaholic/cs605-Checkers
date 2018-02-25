// game.h
// Rohan Weeden
// Created: Feb. 24, 2018

/* Header definitions for game files.
 *
 * The game functions keep track of state variables during a game between two
 * players.
 */

#ifndef CHECKERS_GAME_H
#define CHECKERS_GAME_H

#include "board_state.h"
#include "player.h"


class Game {
private:
    BoardState _board;
    const Player &_redPlayer;
    const Player &_blackPlayer;
    int _current_turn_player;

    int _turn_count;
    bool _has_winner;

    void _setupStartingBoard();
    void _endTurn();

public:
    Game(const Player & red, const Player & black);

    const BoardState & getBoard() const { return _board; }
    int getTurnCount() const { return _turn_count; }

    /* hasWinner
     *
     * Returns true if there is a winner, false otherwise.
     * The winner is the current player
     */
    bool hasWinner() const { return _has_winner; }

    /* getCurrentPlayer
     *
     * Returns the player who is going to make a move next.
     * Either RED_PLAYER or BLACK_PLAYER.
     */
    int getCurrentPlayer() const { return _current_turn_player; }


    /* playGame
     *
     * Plays through an entire game until a winner is found
     */
    void playGame();


    /* playTurn
     *
     * Plays only one turn. This can be called repeatedly to play out the whole
     * game. Useful for clients that want to display the state of the game after
     * each turn.
     */
    void playTurn();


    /* reset
     *
     * Sets the game to a clean starting position.
     */
    void reset();
};


#endif
