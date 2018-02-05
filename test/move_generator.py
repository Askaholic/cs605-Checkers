# move_generator.py
# Rohan Weeden
# Created: Feb. 5, 2018

# Test cases for functions that generate moves

from unittest import TestCase
import board_funcs as bf


class TestMoveGenerator(TestCase):

    def test_starting_board_red(self):
        board = 'rrrrrrrrrrrr11111111bbbbbbbbbbbb'
        moves = bf.get_possible_moves(board, 0)
        expected_moves = [
            (8, 12), (8, 13),
            (9, 13), (9, 14),
            (10, 14), (10, 15),
            (11, 15)
        ]
        self.assertEqual(expected_moves, moves)

    def test_starting_board_black(self):
        board = 'rrrrrrrrrrrr11111111bbbbbbbbbbbb'
        moves = bf.get_possible_moves(board, 1)
        expected_moves = [
            (20, 16),
            (21, 16), (21, 17),
            (22, 17), (22, 18),
            (23, 18), (23, 19)
        ]
        self.assertEqual(expected_moves, moves)
