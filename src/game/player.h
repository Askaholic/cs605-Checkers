// player.h
// Rohan Weeden
// Created: Feb. 24, 2018

// Header definitions for player objects


#ifndef CHECKERS_PLAYER_H
#define CHECKERS_PLAYER_H

#include "asknet4.h"
#include "board_state.h"
#include "jump_generator.h"
#include <ctime>

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


/* class UnPlayer
 *
 * Player implementation which forfeits the game. Useful as a placeholder.
 */
class UnPlayer : public Player {
public:
    UnPlayer(int color):Player(color) {}

    BoardState takeMove(const BoardState & board) override;
};


/* class RandomPlayer
 *
 * Player implementation which chooses moves at random
 */
class RandomPlayer : public Player {
private:
    JumpGenerator gen;

public:
    RandomPlayer(int color):Player(color) {
        std::srand(std::time(nullptr));
    }

    BoardState takeMove(const BoardState & board) override;
};


/* class PieceCountPlayer
 *
 * Player implementation which does a minimax search with piece count evaluation
 * function. Default search depth is 4.
 */
class PieceCountPlayer : public Player {
private:
    size_t _depth = 4;

public:
    PieceCountPlayer (int color):Player(color) {}
    PieceCountPlayer (int color, size_t depth):Player(color), _depth(depth) {}

    BoardState takeMove(const BoardState & board) override;
};


/* class AIPlayer
 *
 * Player implementation which uses a minimax search with neural network as
 * board evaluation function.
 */
class AIPlayer : public Player {
private:
    Network4 _net;
    size_t _depth = 4;

public:
    AIPlayer (int color, const Network4 & net):Player(color), _net(net) {}
    AIPlayer (int color, const Network4 & net, size_t depth):Player(color), _net(net), _depth(depth) {}

    BoardState takeMove(const BoardState & board) override;
    float evaluate(const BoardState & board, int player);
};


#endif
