#!/usr/bin/env python3.6

# Jacob McKenna, Rohan Weeden
# UAF CS605 AI
# Created: Jan. 19, 2018

# Board class and object
from board import Board
import sys
from time import time

from pyglet import clock
from gui import CheckersGame


class GameBoard(object):
	"""docstring for GameBoard."""
	def __init__(self):
		self.board = Board()
		self.turns = 0

		# Visual testing
		print()

		# print('initial board')
		# print()
		# self.board.printBoard()
		# print()
		# print('initial board')

		self.timeout = None

	def update(self, dt):
		if self.timeout:
			if self.timeout > time():
				return
			else:
				self.timeout = None

		if self.turns < 100:
			# print('move', self.turns)
			self.board.moveGenerator()
			self.turns += 1
			self.timeout = time() + .8


if __name__ == '__main__':
	game = CheckersGame(GameBoard())
	clock.schedule_interval(game.update, 1/60.0)

	game.run()
