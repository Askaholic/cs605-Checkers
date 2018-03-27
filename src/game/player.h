// player.h
// Rohan Weeden
// Created: Feb. 24, 2018

// Header definitions for player objects


#ifndef CHECKERS_PLAYER_H
#define CHECKERS_PLAYER_H

#include "board_state.h"

/* class Player
 *
 * Abstract base class for player object.
 */
class Player {
protected:
    int _color_id;

public:

    /* Constructor
     *
     * Takes the player color designation. Either RED_PLAYER or BLACK_PLAYER.
     */
    Player(int player):_color_id(player) {}


    /* takeMove
     *
     * Takes a current BoardState and decides which move the player wants to make.
     * Returns a new BoardState representing the board after the move has been
     * taken.
     *
     * If the player has no moves left to make (aka. they've lost), then it just
     * returns a copy of board.
     */
    virtual BoardState takeMove(const BoardState & board) = 0;

    virtual ~Player() = default; 

};

#endif