# game.py
# Rohan Weeden
# Created: Jan. 31, 2018

# Game class for keeping track of high level game info

from board import Board, RED_PLAYER, BLACK_PLAYER
from time import time
import skynet
import sys


class Game(object):
    def __init__(self):
        self.board = Board()
        self.turns = 0
        self.player = BLACK_PLAYER
        self.timeout = None
        self.all_jumps_taken = False
        self.winner = None

    def end_turn(self):
        self.board.current_turn_player = RED_PLAYER if self.player == BLACK_PLAYER else BLACK_PLAYER
        self.turns += 1
        self.timeout = time() + .3
        self.all_jumps_taken = False

    def set_draw(self):
        self.winner = '--DRAW--'

    def end_game(self):
        self.turns == 300

    def check_winner(self):

        all_moves = self.board.get_all_moves()
        all_jumps = self.board.get_all_jumps()

        if all_moves == [] and all_jumps == []:

            self.winner = not self.board.current_turn_player
            self.end_game()

    def player_turn(self, from_, to_):
        self.check_winner()
        if not self.player == self.board.current_turn_player:
            return

        if len(self.board.get_all_jumps()) > 0:
            if not self.board.is_valid_jump(from_, to_):
                return

            self.board.take_jump(from_, to_)
            if len(self.board.get_all_jumps()) == 0:
                self.end_turn()

        if not self.all_jumps_taken:
            if not self.board.is_valid_move(from_, to_):
                return

            self.board.take_move(self.board.board, from_, to_)

        self.end_turn()
        self.check_winner()

    def update(self, dt):
        if self.timeout:
            if self.timeout > time():
                return
            else:
                self.timeout = None

        if self.turns < 200:
            if self.player != self.board.current_turn_player:
                self.board.ai_turn()
                self.board.current_turn_player = BLACK_PLAYER if self.player == BLACK_PLAYER else RED_PLAYER
                self.check_winner()
                self.turns += 1

        elif self.turns == 200:
            self.winner = not self.board.current_turn_player
        else:
            self.set_draw()


class NetworkGame(Game):
    def __init__(self, name, player):
        super(NetworkGame, self).__init__()
        self.name = name
        self.player = player

        resp = skynet.info_game(name)
        self.board.board = self.board.string_to_board(resp['boards'][-1])


    def update(self, dt):
        if self.winner is not None:
            return


        if self.turns < 200:
            if self.player == self.board.current_turn_player:
                print('making ai turn')
                self.board.ai_turn()
                self.board.current_turn_player = BLACK_PLAYER if self.player == BLACK_PLAYER else RED_PLAYER
                self.check_winner()
                self.turns += 1
                skynet.play_turn(self.name, self.board.board_to_string(self.board.board))
            else:
                print('checking')
                resp = skynet.info_game(self.name)
                board = self.board.board_to_string(self.board.board)
                if resp['boards'][-1] == board:
                    return
                self.board.board = self.board.string_to_board(resp['boards'][-1])

        elif self.turns == 200:
            self.winner = not self.board.current_turn_player
        else:
            self.set_draw()
