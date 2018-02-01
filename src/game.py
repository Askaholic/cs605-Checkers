# game.py
# Rohan Weeden
# Created: Jan. 31, 2018

# Game class for keeping track of high level game info

from board import Board
from time import time

class Game(object):
    def __init__(self):
        self.board = Board()
        self.turns = 0
        self.player = 0

        self.timeout = None

    def update(self, dt):
        if self.timeout:
            if self.timeout > time():
                return
            else:
                self.timeout = None

        if self.turns < 200:
            self.board.moveGenerator()
            self.turns += 1
            self.timeout = time() + .8
