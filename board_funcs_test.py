import board_funcs as bf

# print(bf.test('Board size is'))
# bf.setup_board()
# print(bf.get_board())
# bf.time_boards()

def printBoard(some_board):
    board = ''
    board += '\n'
    odd = False

    for tile in range(32):
        board_element = some_board[tile]
        if board_element == '':
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

starting_board = 'rrrrrrrrrrrr11111111bbbbbbbbbbbb'

print("Possible moves")
print(bf.get_possible_moves(starting_board, 0))
bf.setup_network()
print("Board value: {}".format(bf.evaluate_board(starting_board)))

print("Min Max search with piece_count")
depth = 9
print("Depth: ", depth)
board, score = bf.min_max_search(starting_board, 0, depth)
print("Score: ", score)
printBoard(board)
