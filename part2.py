import board_funcs as bf
bf.setup_network()

def printBoard(some_board):
    board = ''
    board += '\n'
    odd = False

    for tile in range(32):
        board_element = some_board[tile]
        if board_element == '' or board_element == '1':
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
print("Evaluating")
printBoard(starting_board)
print("Value: ", bf.evaluate_board(starting_board))
