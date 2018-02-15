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

# print("Possible moves")
# print(bf.get_possible_moves(starting_board, 0))
# bf.setup_network()
# print("Board value: {}".format(bf.evaluate_board(starting_board)))
#
# print("Min Max search with piece_count")
depth = 18
# print("Depth: ", depth)
# board, score = bf.min_max_search(starting_board, 0, depth)
# print("Score: ", score)
# printBoard(board)
#
#
#print("Min Max search with piece_count **no_alloc**")
#print("Depth: ", depth)
#board, score = bf.min_max_no_alloc(starting_board, 0, depth)
#print("Score: ", score)
#printBoard(board)

print("Get possible jumps")
num_jumps = bf.get_possible_jumps(starting_board, 0)
print("Red player number of valid jumps:", num_jumps)

possible_jump = 'rrrrrrrrr1111b111111bbbbbbbbbbbb'
printBoard(possible_jump)
num_jumps = bf.get_possible_jumps(possible_jump, 0)
print("Red player number of valid jumps:", num_jumps)
# branch_factor = 3
# nodes_to_visit = [0]
# max_depth = 3
# tree_nodes = (pow(branch_factor, max_depth + 1) - 1) // 2
# print("treenodes", tree_nodes)
# while nodes_to_visit != []:
#     current = nodes_to_visit[0]
#     nodes_to_visit = nodes_to_visit[1:]
#     for i in range(1, branch_factor + 1):
#         child = current * branch_factor + i
#         if (child < tree_nodes):
#             # print("   Child: ", child)
#             nodes_to_visit.append(child)
#
#     print(current, ' ', end='')
# print()
