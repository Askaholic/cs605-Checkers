#!/usr/bin/env python3.6

# Board width and height
import sys

# RANDOM Gaming!!!!
import random

# Possible move vectors. I can see a pattens for some
# rows, might be able to implement a better approach.
moveTable = {
    # -1 is a position they cannot move. Allows for [2:] list splitting.
    # Move Up 	Move Down
    0: [-1, -1,		-1, 4],
    1: [-1, -1,		4, 5],
    2: [-1, -1,		5, 6],
    3: [-1, -1,		6, 7],

    4: [0, 1,		8, 9],
    5: [1, 2,		9, 10],
    6: [2, 3,		10, 11],
    7: [3, -1,		11, -1],

    8: [-1, 4,		-1, 12],
    9: [4, 5,		12, 13],
    10: [5, 6,		13, 14],
    11: [6, 7,		14, 15],

    12: [8, 9,		16, 17],
    13: [9, 10,		17, 18],
    14: [10, 11,		18, 19],
    15: [11, -1,		19, -1],

    16: [-1, 12,		-1, 20],
    17: [12, 13,		20, 21],
    18: [13, 14,		21, 22],
    19: [14, 15,		22, 23],

    20: [16, 17,		24, 25],
    21: [17, 18,		25, 26],
    22: [18, 19,		26, 27],
    23: [19, -1,		27, -1],

    24: [-1, 20,		-1, 28],
    25: [20, 21,		28, 29],
    26: [21, 22,		29, 30],
    27: [22, 23,		30, 31],

    28: [24, 25,		-1, -1],
    29: [25, 26,		-1, -1],
    30: [26, 27, 	-1, -1],
    31: [27, -1,		-1, -1]

}


class Board(object):

    def __init__(self):

        self.board = ['1']*32
        self.redPlayer = True
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
        self.printBoard()

    def moveGenerator(self):

        if(self.redPlayer):
            print("Red Player")
        else:
            print("Black Player")

        if (self.redPlayer):
            for i in range(32):
                if (self.board[i] == 'r'):
                    possMoves = moveTable[i]

                    # TESTING
                    possMoves2 = moveTable[i][2:]
                    print('Piece at pos', i, 'possible moves', possMoves2)

                    # Moves down the board only due to list slice.
                    for move in possMoves[2:]:

                        if (self.board[move] == '1'):
                            self.board[move] = 'r'
                            self.board[i] = '1'

                            self.redPlayer = False
                            break

                    # RED player has taken their turn.
                    if (not self.redPlayer):
                        break

                # Gone through all the checkers and not a single RED checker can move.
                if (i == 31 and self.redPlayer):
                    # Red cannot make anymore moves!
                    self.winCondition = 0
                    self.winner()
                    return 1

        else:
            for i in range(32):
                if (self.board[i] == 'b'):
                    possMoves = moveTable[i]

                    # Moves up the board only due to list slice.
                    for move in possMoves[:2]:
                        if (self.board[move] == '1'):
                            # Moves, but can move backwards... for now...
                            self.board[move] = 'b'
                            self.board[i] = '1'

                            self.redPlayer = True
                            break

                    # BLACK player has taken their turn.
                    if (self.redPlayer):
                        break

                # Gone through all the checkers and not a single BLACK checker can move.
                if (i == 31 and not self.redPlayer):
                    # Red cannot make anymore moves!
                    self.winCondition = 1
                    self.winner()
                    return 1

        self.printBoard()

    # Don't touch the print function...
    # Actually, it just prints the board.
    # It really does, by adding a space before
    # and after the checker in the appropriate places.
    def printBoard(self):
        board = '\n'

        # odd switch - Switch to next row
        odd = False
        for tile in range(32):
            # since 0 % 4 = 0, skip the first tile, otherwise we
            # add an extra row with 5 or 1, depending on indexing.
            if (tile % 4 == 0 and tile != 0):
                odd = not odd
                board += '\n'

            if (not odd):
                board += self.board[tile] + ' '
            else:
                board += ' ' + self.board[tile]

        # Extra newline at end of board
        board += '\n'
        print(board)
