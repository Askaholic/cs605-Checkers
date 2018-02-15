# game.py
# Jacob McKenna
# Created: Jan. 18, 2018

# Game class for keeping track of high level game info
from random import shuffle

try:
    import board_funcs as bf
    try:
        bf.setup_network()
    except:
        print('No network available')
except ImportError:
    print("Could not import board_funcs C++ library. Please install it!")
    bf = None

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
        self.setup_board()

    # Sets up the pieces.
    def setup_board(self):
        self.winner = None
        self.current_turn_player = RED_PLAYER
        for checker in range(12):
            self.board[checker] = 'r'
            self.board[31 - checker] = 'b'

    def is_valid_index(self, x):
        if x < 0 or x > 31:
            return False
        return True

    def make_king_if_king_space(self, board, to_):
        piece = board[to_]
        # Make red pawns kings
        if to_ in [28, 29, 30, 31] and piece == 'r':
            board[to_] = 'R'
        # Make black pawns kings
        if to_ in [0, 1, 2, 3] and piece == 'b':
            board[to_] = 'B'

    def is_enemy_in_position(self, from_, to_):
        possJumps = [jump for jump in jumpTable[from_] if jump != -1]
        enemyPos = [jump[1] for jump in possJumps if jump[0] == to_]

        # Implicitly check for empty list
        # print('The enemy position is:', enemyPos)
        if not enemyPos:
            return False

        if self.board[enemyPos[0]] not in playerPieces[not self.current_turn_player]:
            return False

        return True

    def get_enemy_position(self, from_, to_):
        possJumps = [jump for jump in jumpTable[from_] if jump != -1]
        enemyPos = [jump[1] for jump in possJumps if jump[0] == to_]
        # enemyPos is a list, should only ever get 1 item
        # print('enemyPos should not be larger than 1', enemyPos)
        return enemyPos[0]

    # Called from GUI
    def take_move(self, board, from_, to_):
        piece = board[from_]
        board[from_] = '1'
        board[to_] = piece

        self.make_king_if_king_space(board, to_)

    def take_jump(self, from_, to_):
        enemyPos = self.get_enemy_position(from_, to_)
        piece = self.board[from_]
        self.board[enemyPos] = '1'
        self.board[from_] = '1'
        self.board[to_] = piece

        self.make_king_if_king_space(self.board, to_)

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
        possJumps = jumpTable[from_]

        if piece == 'r':
            possJumps = [jump for jump in possJumps[2:] if jump != -1]
        elif piece == 'b':
            possJumps = [jump for jump in possJumps[:2] if jump != -1]
        else:
            possJumps = [jump for jump in jumpTable[from_] if jump != -1]

        return possJumps

    def is_valid_move(self, from_, to_):
        if len(self.get_all_jumps()) > 0:
            return False
        if not self.is_valid_index(from_) or not self.is_valid_index(to_):
            return False
        if self.board[to_] != '1':
            return False
        if from_ == to_:
            return False
        if to_ not in moveTable[from_]:
            return False
        if (to_ not in self.get_possible_moves_of_piece(from_)):
            return False

        return True

    def is_valid_jump(self, from_, to_):
        if not self.is_valid_index(from_) or not self.is_valid_index(to_):
            return False
        if self.board[to_] != '1':
            return False
        if not self.is_enemy_in_position(from_, to_):
            return False
        if self.board[self.get_enemy_position(from_, to_)] not in playerPieces[not self.current_turn_player]:
            return False
        if not self.get_possible_jumps_of_piece(from_):
            return False
        if (to_ not in [jump[0] for jump in self.get_possible_jumps_of_piece(from_)]):
            return False

        return True

    def get_all_jumps(self):
        allPossibleJumps = []

        for i in range(32):
            if (self.board[i] not in playerPieces[self.current_turn_player]):
                continue

            possJumps = [jump for jump in jumpTable[i] if jump != -1]

            for jump in possJumps:
                if self.is_valid_jump(i, jump[0]):
                    allPossibleJumps.append((i, jump))

        return allPossibleJumps

    def get_all_moves(self):
        allPossibleMoves = []

        for i in range(32):
            if (self.board[i] not in playerPieces[self.current_turn_player]):
                continue

            possMoves = moveTable[i]

            for move in possMoves:
                if (not self.is_valid_move(i, move)):
                    continue
                allPossibleMoves.append((i, move))

        return allPossibleMoves

    def choose_jump(self, jumps):
        # Always take all jumps.
        # for jump in jumps:
            # self.take_jump()
            # print
        first_jump = jumps[0]
        self.take_jump(first_jump[0], first_jump[1][0])

    def choose_move(self, moves):
        if bf is None:
            # Take the first move if the c++ lib is not available
            best_move = moves[0]
            self.take_move(self.board, best_move[0], best_move[1])
        else:
            board_string = ""
            for c in self.board:
                board_string += c
            print("Using board_funcs")
            board, score = bf.min_max_search_ab(board_string, self.current_turn_player, 4)
            for i in range(len(board)):
                if (board[i] == ''):
                    self.board[i] = '1'
                else:
                    self.board[i] = board[i]

    def simple_search_for_best_move(self, moves):
        best_score = 0
        best_move = moves[0]
        for move in moves:
            board_string = self.board_to_string(self.expand_move(move))
            try:
                score = bf.evaluate_board(board_string)
            except:
                print('No evaluate Board')
                score = 0
            # TODO: check which player's turn it is
            if score > best_score:
                best_score = score
                best_move = move

        return best_move

    def expand_move(self, move):
        board_copy = list(self.board)
        self.take_move(board_copy, move[0], move[1])
        return board_copy

    def board_to_string(self, board):
        string = ""
        for piece in board:
            string += piece
        return string

    def ai_take_all_jumps(self, jumps):
        while len(jumps) > 0:

            # O(n) shuffle, randomizes
            shuffle(jumps)

            first_jump = jumps[0]
            self.take_jump(first_jump[0], first_jump[1][0])

            jumps = self.get_all_jumps()
            print(len(jumps))

    def make_ai_move(self):
        available_jumps = self.get_all_jumps()
        if available_jumps != []:
            self.ai_take_all_jumps(available_jumps)
            return
        else:
            available_moves = self.get_all_moves()
            if available_moves == []:
                # We lost
                # self.winner = RED_PLAYER if self.current_turn_player == BLACK_PLAYER else BLACK_PLAYER
                return
            self.choose_move(available_moves)

    def ai_turn(self):
        self.make_ai_move()
        self.printBoard(self.board)

    def printBoard(self, some_board):
        board = ''
        board += '\n'
        odd = False

        for tile in range(32):
            board_element = some_board[tile]
            if board_element == '1':
                board_element = '.'
            if (tile % 4 == 0 and tile != 0):
                odd = not odd
                board += '\n'

            if (not odd):
                board += ' ' + board_element
            else:
                board += board_element + ' '

        board += '\n'
        print(board)
