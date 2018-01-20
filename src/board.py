#!/usr/bin/env python3.6

# Board width and height

class Board(object):

	def __init__(self):
		
		self.board = [' ']*32


	def setupBoard(self):

		currentTile = 0
		currentRow = 0




	def printBoard(self):
		for x in range(8):
			for y in range(8):

				if (y % 2 == 0):
					print(' ',end = '')
				else:
					print(self.board[y], end= '')

			print()



