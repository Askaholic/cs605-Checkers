# test.py
# Rohan Weeden
# Created: Feb. 4, 2018

# Main unit testing file

from unittest import TestCase, main
import board_funcs as bf


class TestMoveGenerator(TestCase):

    def test_starting_board_red(self):
        board = 'rrrrrrrrrrrr11111111bbbbbbbbbbbb'
        moves = bf.get_possible_moves(board, 1)
        expected_moves = [
            (8, 12), (8, 13),
            (9, 13), (9, 14),
            (10, 14), (10, 15),
            (10, 16)
        ]
        self.assertEqual(expected_moves, moves)

    def test_starting_board_black(self):
        board = 'rrrrrrrrrrrr11111111bbbbbbbbbbbb'
        moves = bf.get_possible_moves(board, 0)
        expected_moves = [
            (20, 16),
            (21, 16), (21, 17),
            (22, 17), (22, 18),
            (23, 18), (23, 19)
        ]
        self.assertEqual(expected_moves, moves)

if __name__ == '__main__':
    main()
