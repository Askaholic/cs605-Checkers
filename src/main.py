#!/usr/bin/env python3.6

# Jacob McKenna, Rohan Weeden
# UAF CS605 AI
# Created: Jan. 19, 2018

# Starts the gui

from board import  RED_PLAYER, BLACK_PLAYER

from pyglet import clock
from gui import CheckersGame
from game import NetworkGame
import skynet

import sys


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Please supply a game name and player color')
        sys.exit(-1)

    name = sys.argv[1]
    player = RED_PLAYER if sys.argv[2].lower() == 'red' else BLACK_PLAYER

    resp = skynet.info_game(name)
    if 'error' in resp:
        print('Game could not be found')
        sys.exit(-1)

    game = CheckersGame(NetworkGame(name, player))
    clock.schedule_interval(game.update, 1/60.0)

    game.run()
