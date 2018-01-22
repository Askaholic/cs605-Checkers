#!/usr/bin/env python3.6

# Board width and height

class Board(object):

	def __init__(self):
		
		self.board = ['1']*32
		self.playerTurn = 0 

	# Sets up the pieces. 
	def setupBoard(self):

		for checker in range(12):
			self.board[checker] = 'r'
			self.board[31 - checker] = 'b'

	def printRawBoard(self):
		print(self.board)

	# Possible move vectors. I can see a pattens for some 
	# rows, might be able to implement a better approach.
	def buildMoveTable(self):
		self.moveTable = {
			0: [4,5],
			1: [5,6],
			2: [6,7],
			3: [7],

			4: [0,8],
			5: [0,1,8,9],
			6: [1,2,9,10],
			7: [2,3,10,11],

			8: [4,5,12,13],
			9: [5,6,13,14],
			10: [6,7,14,15],
			11: [7,15],

			12: [8,16],
			13: [8,9,16,17],
			14: [9,10,17,18],
			15: [10,11,18,19],

			16: [12,13,20,21],
			17: [13,14,21,22],
			18: [14,15,22,23],
			19: [15,23]

		}


	def printBoard(self):

		board = ''

		board += '\n'

		# odd switch - Switch to next row
		odd = False
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
