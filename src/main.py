#!/usr/bin/env python3.6

# Jacob McKenna, Rohan Weeden
# UAF CS605 AI
# Created: Jan. 19, 2018

# Starts the gui

from pyglet import clock
from gui import CheckersGame
from game import NetworkGame
import skynet

import sys


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Please supply a game name')
        sys.exit(-1)

    name = sys.argv[1]

    resp = skynet.info_game(name)
    if 'error' in resp:
        print('Game could not be found')
        sys.exit(-1)

    game = CheckersGame(NetworkGame(name))
    clock.schedule_interval(game.update, 1/60.0)

    game.run()
