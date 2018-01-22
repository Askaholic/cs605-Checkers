#!/usr/bin/env python3.6

# Board width and height

class Board(object):

	def __init__(self):
		
		self.board = ['1']*32


	def setupBoard(self):

		for checker in range(12):
			self.board[checker] = 'r'
			self.board[31 - checker] = 'b'

	def printRawBoard(self):
		print(self.board)


	def printBoard(self):

		board = ''

		board += '\n'

		# odd switch - Switch to next row
		odd = True
		for tile in range(32):

			if (tile % 4 == 0 and tile != 0):

				odd = not odd
				board += '\n'


			if (not odd):
				board += ' ' + self.board[tile]

			else:
				board += self.board[tile] + ' '

		board += '\n'
		
		print(board)
