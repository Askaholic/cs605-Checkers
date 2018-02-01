#!/usr/bin/env python3.6

import sys
import random

moveTable = {
    # -1 is a position they cannot move. Allows for [2:] list splitting.
    # Move Up  Move Down
    0:  [-1,-1,  4,5],
    1:  [-1,-1,  5,6],
    2:  [-1,-1,  6,7],
    3:  [-1,-1,  -1,7],

    4:  [-1,0,   -1,8],
    5:  [0,1,    8,9],
    6:  [1,2,    9,10],
    7:  [2,3,    10,11],

    8:  [4,5,    12,13],
    9:  [5,6,    13,14],
    10: [6,7,    14,15],
    11: [7,-1,  15,-1],

    12: [-1,8,  -1,16],
    13: [8,9,    16,17],
    14: [9,10,  17,18],
    15: [10,11,  18,19],

    16: [12,13,  20,21],
    17: [13,14,  21,22],
    18: [14,15,  22,23],
    19: [15,-1,  23,-1],

    20: [-1,16,  -1,24],
    21: [16,17,  24,25],
    22: [17,18,  25,26],
    23: [18,19,  26,27],

    24: [20,21,  28,29],
    25: [21,22,  29,30],
    26: [22,23,  30,31],
    27: [23,-1,  31,-1],

    28: [-1,24  -1,-1],
    29: [24,25,  -1,-1],
    30: [25,26,  -1,-1],
    31: [26,27,  -1,-1]

}

jumpTable = {

	0: 	[-1,-1,        -1,(9,5)],
	1: 	[-1,-1,		(8,5),(10,6)],
	2: 	[-1,-1,		(9,6),(11,7)],
	3: 	[-1,-1,		(10,7),-1],

	4: 	[-1,-1,		-1,(13,8)],
	5: 	[-1,-1,		(12,8),(14,9)],
	6: 	[-1,-1,		(13,9),(15,10)],
	7: 	[-1,-1,		(14,10),-1],

	8: 	[-1,(1,5),		    -1,(17,13)],
	9: 	[(0,5),(2,6),		(16,13),(18,14)],
	10: [(1,6),(3,7),	     (17,14),(19,15)],
	11: [(2,7),-1,		      (18,15),-1],

	12: [-1,(5,8),		-1,(21,16)],
	13: [(4,8),(6,9),		(20,16),(22,17)],
	14: [(5,9),(7,10),		(21,17),(23,18)],
	15: [(6,10),-1,		(22,18),-1],

	16: [-1,(9, 13),		-1,(25, 21)],
	17: [(8, 13),(10, 14),		(24, 21),(26, 22)],
	18: [(9, 14),(11, 15),		(25, 22),(27, 23)],
	19: [(10, 15),-1,		(26, 23),-1],

	20: [-1,(13, 16),		-1,(29, 24)],
	21: [(12, 16),(14, 17),		(28, 24),(30, 25)],
	22:	[(13, 17),(15, 18),		(29, 25), (31, 26)],
	23: [(14, 18),-1,		(30, 26),-1],

	24:	[(17, 21),-1,		-1,-1],
	25:	[(16, 21),(18, 22),		-1,-1],
	26: [(17, 22),(19, 23),		-1,-1],
	27: [(18, 23),-1,		-1,-1],

	28:[-1,(21, 24),		-1,-1],
	29:[(20, 24),(22, 25),		-1,-1],
	30:[(21, 25),(23, 26),		-1,-1],
	31:[(22, 26),-1,		-1,-1],

}

RED_PLAYER = 0
BLACK_PLAYER = 1

playerColors = {
    
    0: ['r', 'R'],
    1: ['b', 'B']

}


class Board(object):

    def __init__(self):
        self.board = ['1']*32
        self.players = {
            0: 'red',
            1: 'black'
        }
        self.current_turn_player = RED_PLAYER
        self.winCondition = -1

        self.setupBoard()       
        self.printBoard()

    # Sets up the pieces.
    def setupBoard(self):
        for checker in range(12):
            self.board[checker] = 'r'
            self.board[31 - checker] = 'b'


    def is_winner(self):

       if (self.winCondition == 0):
         print('Winner is Black - Red cannot make anymore moves!')

       elif (self.winCondition == 1):
         print('Winner is red - Black canoot make anymore moves!')

       elif (self.winCondition == 2):
         print('Black is the winner - Red has no more pieces!')

       elif (self.winCondition == 3):
         print('Red is the winner - Black has no more pieces!')


    def generateAllPossibleMoves(self, index, moves):

        allPossMoves = []

        # print(moves)
        for move in moves:
            if(self.board[move] == '1'):
                allPossMoves.append(move)

        if(len(allPossMoves) == 0):
            return []

        return [{index: allPossMoves}]

    def is_valid_index(self, x):
        if x < 0 or x > 31:
            return False

        return True

    def is_valid_move(self, from_, to_):
        if not self.is_valid_index(from_) or not self.is_valid_index(to_):
            return False

        if self.board[to_] != '1':
            return False

        piece = self.board[from_]
        possMoves = moveTable[from_]

        if piece in ['r', 'R']:
            possMoves = possMoves[2:]
        else:
            possMoves = possMoves[:2]

        if (to_ not in possMoves):
            return False

        return True

    def move(self, from_, to_):
        piece = self.board[from_]
        self.board[from_] = '1'
        self.board[to_] = piece

    def is_move(self, from_, to_):
        if to_ not in moveTable[from_]:
            return False 
        return True

    def jump(self, from_, to_):
        piece = self.board[from_]
        self.board[from_] = '1'
        self.board[to_] = piece

    def is_jump(self, from_, to_):
        possJumps = [jump for jump in jumpTable[from_] if jump != -1]

        print('There are the possible jump:', possJumps, 'from:', from_, 'on players turn:' , self.current_turn_player)
        # print(playerColors[self.current_turn_player])
        # print(playerColors[not self.current_turn_player])
        # print(playerColors[self.current_turn_player])
        for jump in possJumps:
            print(jump,from_, jump[0], to_, jump[1])

            print('Bool please work:',playerColors[not self.current_turn_player])
            if jump[0] == to_ and jump[1] in playerColors[self.current_turn_player]:
                print('am i here yet')
                return True


        return False

    # Called from GUI 
    def take_move(self, from_, to_):
        if (not self.is_valid_move(from_, to_)):
            return

        if self.is_jump(from_, to_): 
            self.jump(from_, to_)
            return

        if self.is_move(from_, to_):
            self.move(from_, to_)
            return

    def make_current_player_move(self):
        for i in range(32):

            if (self.board[i] not in playerColors[self.current_turn_player]):
                continue

            possMoves = moveTable[i]

            if (self.current_turn_player == RED_PLAYER):
                possMoves = possMoves[2:] 
            else:
                possMoves = possMoves[:2]

            for move in possMoves:
                if (not self.is_valid_move(i, move)):
                    continue
            
                self.take_move(i, move)
                return 


    # Rename 
    def moveGenerator(self):
        # Visual Testing
        # print('Am i alternating player',self.current_turn_player)
        if(self.current_turn_player == RED_PLAYER):
            print("Red Player")
        else:
            print("Black Player")

        self.make_current_player_move()
        self.printBoard()

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
