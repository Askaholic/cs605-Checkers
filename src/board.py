#!/usr/bin/env python3.6

# Board width and height

class Board(object):

	def __init__(self):

		
		self.board = [[' ' for x in range(8)] for y in range(8)]


	def setupBoard(self):

		currentTile = 0
		currentRow = 0

		for row in self.board:

			if (currentRow % 2 == 0):

				for tile in row:
					
					if (tile == ' ' and currentTile % 2 == 0):
						row[currentTile] = 'b'

					currentTile += 1

			else:

				for tile in row:
					
					if (tile == ' ' and currentTile % 2 == 1):
						row[currentTile] = 'r'

					currentTile += 1


			currentTile = 0
			currentRow += 1
			


	def printBoard(self):
		for row in self.board:
			print(row)
