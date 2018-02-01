#!/usr/bin/env python3.6

# Jacob McKenna, Rohan Weeden
# UAF CS605 AI
# Created: Jan. 19, 2018

# Starts the gui

from pyglet import clock
from gui import CheckersGame
from game import Game


if __name__ == '__main__':
    game = CheckersGame(Game())
    clock.schedule_interval(game.update, 1/60.0)
    # b = Board()
    # b.generateJumpTable()

    game.run()
