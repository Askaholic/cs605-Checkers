#!/usr/bin/env python3.6

import sys
import random

moveTable = {
	# -1 is a position they cannot move. Allows for [2:] list splitting.
	# Move Up 	Move Down
	0: [-1,-1,		4,5],
	1: [-1,-1,		5,6],
	2: [-1,-1,		6,7],
	3: [-1,-1,		-1,7],

	4: [-1,0,		-1,8],
	5: [0,1,		8,9],
	6: [1,2,		9,10],
	7: [2,3,		10,11],

	8: [4,5,		12,13],
	9: [5,6,		13,14],
	10: [6,7,		14,15],
	11: [7,-1,		15,-1],

	12: [-1,8,		-1,16],
	13: [8,9,		16,17],
	14: [9,10,		17,18],
	15: [10,11,		18,19],

	16: [12,13,		20,21],
	17: [13,14,		21,22],
	18: [14,15,		22,23],
	19: [15,-1,		23,-1],

	20: [-1,16,		-1,24],
	21: [16,17,		24,25],
	22: [17,18,		25,26],
	23: [18,19,		26,27],

	24: [20,21,		28,29],
	25: [21,22,		29,30],
	26: [22,23,		30,31],
	27: [23,-1,		31,-1],

	28: [-1,24		-1,-1],
	29: [24,25,		-1,-1],
	30: [25,26, 	-1,-1],
	31: [26,27,		-1,-1]
}

jumpTable = {
	0: [-1,-1,		-1,9],
	1: [-1,-1,		8,10],
	2: [-1,-1,		9,11],
	3: [-1,-1,		10,-1],

	4: [-1,-1,		-1,13],
	5: [-1,-1,		12,14],
	6: [-1,-1,		1], # Not finished!!!
}

class Board(object):

	def __init__(self):
		self.board = ['1']*32
		self.players = {
			0: 'red',
			1: 'black'
		}
		self.is_red_players_turn = True
		self.winCondition = -1

		self.setupBoard()

	# Sets up the pieces.
	def setupBoard(self):
		for checker in range(12):
			self.board[checker] = 'r'
			self.board[31 - checker] = 'b'

	# Debugging function. May be dropped soon.
	def printRawBoard(self):
		print(self.board)
		print(sys.getsizeof(self.moveTable))

	def generateJumpTable(self):
		for i in range(32):

			tl = i - 5
			tr = i - 4
			bl = i + 3
			br = i + 4

			odd = not False

			if (i != 0 and (i % 4 == 0)):
				odd = not odd

			if (not odd):
				pass


			if (tl < 0):
				tl = -1
			if (tr < 0):
				tr = -1
			if (bl > 31):
				bl = -1
			if (br > 31):
				br = -1

			print(i, ":", tl,tr,bl,br)


	def winner(self):

		if (self.winCondition == 0):
			print('Winner is Black - Red cannot make anymore moves!')

		elif (self.winCondition == 1):
			print('Winner is red - Black canoot make anymore moves!')

		elif (self.winCondition == 2):
			print('Black is the winner - Red has no more pieces!')

		elif (self.winCondition == 3):
			print('Red is the winner - Black has no more pieces!')

		# Print the winning board!!!
		# self.printBoard()

	def playerMove(self):
		return 1

	def generateAllPossibleMoves(self, index, moves):

		allPossMoves = []

		# print(moves)
		for move in moves:
			if(self.board[move] == '1'):
				allPossMoves.append(move)

		if(len(allPossMoves) == 0):
			return []


		return [{index: allPossMoves}]

	def isValidMove(self, FROM, TO):
		if TO < 0 or TO > 31:
			return False
		if FROM < 0 or FROM > 31:
			return False
		if self.board[TO] != '1':
			return False

		return True


	def take_move(self, FROM, TO):

		if (not self.isValidMove(FROM, TO)):
			return None

		piece = self.board[FROM]
		self.board[FROM] = '1'
		self.board[TO] = piece


	def make_black_move(self):

		moves = []

		for i in range(32):

			if (self.board[i] == 'b'):

				moves.extend(self.generateAllPossibleMoves(i, moveTable[i][:2]))
				print(moves)

		for move in range(len(moves)):
			print(moves[move])
			# 	# Moves up the board only due to list slice.
			# 	for move in moveTable[i][:2]:

			# 		if (self.board[move] == '1'):

			# 			# Moves, but can move backwards... for now...
			# 			self.board[move] = 'b'
			# 			self.board[i] = '1'

			# 			self.is_red_players_turn = True
			# 			break

			# 	# BLACK player has taken their turn.
			# 	if (self.is_red_players_turn):
			# 		break

			# # Gone through all the checkers and not a single BLACK checker can move.
			# if (i == 31 and not self.is_red_players_turn):
			# 	# Red cannot make anymore moves!
			# 	self.winCondition = 1
			# 	self.winner()
			# 	return 1


	def make_red_move(self):

		for i in range(32):

			if (self.board[i] == 'r'):

				possMoves = moveTable[i]

				# Moves down the board only due to list slice.
				for move in possMoves[2:]:

					if (self.board[move] == '1'):

						self.board[move] = 'r'
						self.board[i] = '1'

						self.is_red_players_turn = False
						break

				# RED player has taken their turn.
				if (not self.is_red_players_turn):
					break

			# Gone through all the checkers and not a single RED checker can move.
			if (i == 31 and self.is_red_players_turn):
				# Red cannot make anymore moves!
				self.winCondition = 0
				self.winner()
				return 1

	def moveGenerator(self):

		# Visual Testing
		if(self.is_red_players_turn):
			print("Red Player")
		else:
			print("Black Player")


		if (self.is_red_players_turn):
			# self.generateAllPossibleMoves()
			self.make_red_move()

		else:
			# self.generateAllPossibleMoves()
			self.make_black_move()

		self.printBoard()

	# Don't touch the print function...
	# Actually, it just prints the board.
	# It really does, by adding a space before
	# and after the checker in the appropriate places.
	def printBoard(self):

		board = ''

		board += '\n'

		# odd switch - Switch to next row
		odd = False


		for tile in range(32):

			# since 0 % 4 = 0, skip the first tile, otherwise we
			# add an extra row with 5 or 1, depending on indexing.
			if (tile % 4 == 0 and tile != 0):

				odd = not odd
				board += '\n'


			if (not odd):
				board += ' ' + self.board[tile]

			else:
				board += self.board[tile] + ' '


		# Extra newline at end of board
		board += '\n'

		print(board)
