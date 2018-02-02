# game.py
# Jacob McKenna
# Created: Jan. 18, 2018

# Game class for keeping track of high level game info
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

    28: [-1,24,  -1,-1],
    29: [24,25,  -1,-1],
    30: [25,26,  -1,-1],
    31: [26,27,  -1,-1]

}

jumpTable = {
    #  from_: (to_, enemy)
    #  -1 = non-movable space 
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

playerPieces = {
    
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

        self.setup_board()       
        self.printBoard()

    # Sets up the pieces.
    def setup_board(self):
        for checker in range(12):
            self.board[checker] = 'r'
            self.board[31 - checker] = 'b'

    def is_valid_index(self, x):
        if x < 0 or x > 31:
            return False
        return True

    def make_king(self, to_):
        # Make red pawns kings
        if to_ in [28,29,30,31]:
            self.board[to_] = 'R'
        # Make black pawns kings
        if to_ in [0,1,2,3]:
            self.board[to_] = 'B'


    def get_enemy_position(self, from_, to_):
        possJumps = [jump for jump in jumpTable[from_] if jump != -1]
        enemyPos = [jump[1] for jump in possJumps if jump[0] == to_]
        return enemyPos[0]

    # Called from GUI 
    def take_move(self, from_, to_):
        piece = self.board[from_]
        self.board[from_] = '1'
        self.board[to_] = piece

    def take_jump(self, from_, to_):
        enemyPos = self.get_enemy_position(from_, to_)
        piece = self.board[from_]
        self.board[enemyPos] = '1'
        self.board[from_] = '1'
        self.board[to_] = piece

    def get_possible_moves_of_piece(self, from_):
        piece = self.board[from_]
        possMoves = moveTable[from_] 

        if piece == 'r':
            possMoves = possMoves[2:]
        elif piece == 'b':
            possMoves = possMoves[:2]

        return possMoves

    def get_possible_jumps_of_piece(self, from_):
        piece = self.board[from_]
        possJumps = [jump for jump in jumpTable[from_] if jump != -1]

        if piece == 'r':
            possJumps = [jump for jump in possJumps[2:] if jump != -1]
        elif piece == 'b':
            possJumps = [jump for jump in possJumps[:2] if jump != -1]

        return possJumps

    def is_valid_move(self, from_, to_):
        if not self.is_valid_index(from_) or not self.is_valid_index(to_):
            return False
        if self.board[to_] != '1':
            return False
        if (to_ not in self.get_possible_moves_of_piece(from_)):
            return False

        return True

    # def is_enemy(self, from_, to_):

    def is_valid_jump(self, from_, to_):
        if not self.is_valid_index(from_) or not self.is_valid_index(to_):
            return False
        if self.board[to_] != '1':
            return False
        if self.board[self.get_enemy_position(from_, to_)] not in playerPieces[not self.current_turn_player]:
            return False

        # print('from_:', from_, ' to_:', to_)
        # print('Possible jumps:',self.get_possible_jumps_of_piece(from_))
        # print('I think this is ...', [jump[0] for jump in self.get_possible_jumps_of_piece(from_)])
        if (to_ not in [jump[0] for jump in self.get_possible_jumps_of_piece(from_)]):
            return False

        return True

    def get_all_jumps(self):

        allPossibleJumps = []

        for i in range(32):
            if (self.board[i] not in playerPieces[self.current_turn_player]):
                continue

            possJumps = [jump for jump in jumpTable[i] if jump != -1]
            # print(possJumps)

            for jump in possJumps:
                if self.is_valid_jump(i, jump[0]):
                    # print(jump)
                    allPossibleJumps.append((jump))

        return allPossibleJumps


    def generate_all_current_player_moves(self):
        for i in range(32):
            if (self.board[i] not in playerPieces[self.current_turn_player]):
                continue

        pass

    def generate_all_current_player_jumps(self):
        pass

    def make_current_player_move(self):

        # Visual Testing
        print('=='*20)    
        for i in range(32):

            if (self.board[i] not in playerPieces[self.current_turn_player]):
                continue

            possMoves = moveTable[i]

            if (self.current_turn_player == RED_PLAYER):

                piece = self.board[i]

                if piece == 'r':
                    possMoves = possMoves[2:]

            else:
                piece = self.board[i]

                if piece == 'b':
                    possMoves = possMoves[2:]

            print('All possible moves', possMoves, 'for piece', i)
            for move in possMoves:
                if (not self.is_valid_move(i, move)):
                    continue
            
                self.take_move(i, move)
                return 



    # Rename 
    def ai_move(self):

        # self.generate_all_current_player_jumps()
        # self.generate_all_current_player_moves()
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

        board += '\n'
        print(board)
