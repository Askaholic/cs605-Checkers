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


/* class RandomSearchPlayer
 *
 * Player implementation which does a minimax search but uses a random number
 * generator as a board evaluation function.
 */
class RandomSearchPlayer : public Player {
private:
    size_t _depth = 4;

public:
    RandomSearchPlayer (int color):Player(color) {
        std::srand(std::time(nullptr));
    }

    RandomSearchPlayer (int color, size_t depth):Player(color), _depth(depth) {}

    BoardState takeMove(const BoardState & board) override;

    float evaluate(const BoardState & board, int player);
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

protected:
    virtual std::vector<float> _boardStateToNetworkInputs(const BoardState & board, const Network4 & net, int player);

public:
    AIPlayer (int color, const Network4 & net):Player(color), _net(net) {}
    AIPlayer (int color, const Network4 & net, size_t depth):Player(color), _net(net), _depth(depth) {}

    BoardState takeMove(const BoardState & board) override;
    float evaluate(const BoardState & board, int player);
};


/* class AIPlayerWithPieceCount
 *
 * Same as AIPlayer but with an added piece count input at the starting layer.
 */
class AIPlayerWithPieceCount : public AIPlayer {
protected:
    virtual std::vector<float> _boardStateToNetworkInputs(const BoardState & board, const Network4 & net, int player) override;

public:
    AIPlayerWithPieceCount (int color, const Network4 & net):AIPlayer(color, net) {}
    AIPlayerWithPieceCount (int color, const Network4 & net, size_t depth):AIPlayer(color, net, depth) {}
};


/* class AIPlayer3Net
 *
 * Player implementation which uses a minimax search with neural network as
 * board evaluation function. Uses 3 different networks for 3 stages of the game.
 * Beginning: 24-17 pieces, Middle: 16-9 pieces, End: 8-0 pieces.
 */
class AIPlayer3Net : public Player {
private:
    Network4 _beg;
    Network4 _mid;
    Network4 _end;
    size_t _depth = 4;

protected:
    virtual float _evaluateWithNetwork(const BoardState & board, int player, Network4 & net);

public:
    AIPlayer3Net (int color, const Network4 & beg, const Network4 & mid,
                  const Network4 & end):Player(color), _beg(beg), _mid(mid),
                  _end(end) {}

    AIPlayer3Net (int color, const Network4 & beg, const Network4 & mid,
                const Network4 & end, size_t depth):Player(color), _beg(beg),
                _mid(mid), _end(end), _depth(depth) {}

    BoardState takeMove(const BoardState & board) override;
    float evaluate(const BoardState & board, int player);
};

/* class AIPlayer3NetWithPieceCount
 *
 * Same as AIPlayer3Net but with an added piece count input at the starting layer.
 */
class AIPlayer3NetWithPieceCount : public AIPlayer3Net {
protected:
    float _evaluateWithNetwork(const BoardState & board, int player, Network4 & net) override;

public:
    AIPlayer3NetWithPieceCount (int color, const Network4 & beg, const Network4 & mid,
                  const Network4 & end):AIPlayer3Net(color, beg, mid, end) {}

    AIPlayer3NetWithPieceCount (int color, const Network4 & beg, const Network4 & mid,
                const Network4 & end, size_t depth):AIPlayer3Net(color, beg, mid, end, depth) {}
};


/* class ExternalPlayer
 *
 * Player implementation which gets moves from an external source. Used to play
 * against someone over a network connection.
 */
class ExternalPlayer : public Player {
private:
    BoardState _next_board;

public:
    ExternalPlayer(int color):Player(color) { }

    BoardState takeMove(const BoardState & board) override {
        return _next_board;
    }

    void setMove(const BoardState & next_board) {
        _next_board = next_board;
    }
};

#endif
