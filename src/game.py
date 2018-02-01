# game.py
# Rohan Weeden
# Created: Jan. 31, 2018

# Game class for keeping track of high level game info

from board import Board, RED_PLAYER, BLACK_PLAYER
from time import time

class Game(object):
    def __init__(self):
        self.board = Board()
        self.turns = 0
        self.player = BLACK_PLAYER
        self.timeout = None

    def player_take_move(self, from_, to):
        if not self.board.is_valid_move(from_, to) or not self.player == self.board.current_turn_player:
            return

        self.board.take_move(from_, to)
        self.board.current_turn_player = RED_PLAYER if self.player == BLACK_PLAYER else BLACK_PLAYER
        self.turns += 1
        self.timeout = time() + .3

    def update(self, dt):
        if self.timeout:
            if self.timeout > time():
                return
            else:
                self.timeout = None

        if self.turns < 200:
            if self.player != self.board.current_turn_player:
                self.board.moveGenerator()
                self.board.current_turn_player = BLACK_PLAYER if self.player == BLACK_PLAYER else RED_PLAYER
                self.turns += 1
