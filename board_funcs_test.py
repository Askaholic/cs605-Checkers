import board_funcs as bf

print(bf.test('Board size is'))
bf.setup_board()
print(bf.get_board())
bf.time_boards()

print("Possible moves")
print(bf.get_possible_moves("rrrrrrrrrrrr11111111bbbbbbbbbbbb", 0))
