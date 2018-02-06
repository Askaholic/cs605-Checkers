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
        self.jumpTaken = False

    def player_take_all_available_jumps(self, from_, to_):
        if len(self.board.get_all_jumps()) > 0:
            print('Possibels Jumps:', len(self.board.get_all_jumps()))
            if not self.board.is_valid_jump(from_, to_):
                return

            self.board.take_jump(from_, to_)
            return

    def end_turn(self):
        self.board.current_turn_player = RED_PLAYER if self.player == BLACK_PLAYER else BLACK_PLAYER
        self.turns += 1
        self.timeout = time() + .3
        self.jumpTaken = False

    def player_turn(self, from_, to_):
        if not self.player == self.board.current_turn_player:
            return

        if len(self.board.get_all_jumps()) > 0:
            self.player_take_all_available_jumps(from_, to_)
            self.jumpTaken = True
            self.end_turn()

        if not self.jumpTaken:
            if not self.board.is_valid_move(from_, to_):
                return

            self.board.take_move(from_, to_)
            self.end_turn()

            

    def update(self, dt):
        if self.timeout:
            if self.timeout > time():
                return
            else:
                self.timeout = None

        if self.turns < 200:
            if self.player != self.board.current_turn_player:
                self.board.play_ai()
                self.board.current_turn_player = BLACK_PLAYER if self.player == BLACK_PLAYER else RED_PLAYER
                self.turns += 1
