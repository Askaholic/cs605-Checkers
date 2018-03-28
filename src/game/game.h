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
#include "jump_generator.h"
#include "player.h"
#include <random>

class Game {
private:
    std::random_device _rd;
    JumpGenerator _moveGen;

    BoardState _board;
    Player &_redPlayer;
    Player &_blackPlayer;
    int _current_turn_player;

    size_t _turn_count;
    size_t _max_turn_count = 200;
    bool _has_winner;

    void _setupStartingBoard();
    void _endTurn();
    void _takeMove(const BoardState & board);

public:
    Game(Player & red, Player & black);

    const BoardState & getBoard() const { return _board; }
    int getTurnCount() const { return _turn_count; }

    /* hasWinner
     *
     * Returns true if there is a winner, false otherwise.
     * The winner is the current player
     */
    bool hasWinner() const { return _has_winner; }

    /* getWinner()
     *
     * Returns the player who won, or -1 if there was a draw.
     * Either RED_PLAYER, BLACK_PLAYER or -1
     */
    int getWinner() const;

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

    /* randomizeOpeningMoves
     *
     * Starts the game off by making the first few moves randomly. Does not
     * consider jumps, so it should only be called at the beginning of a game.
     */
    void randomizeOpeningMoves(int numMoves);
};


#endif
