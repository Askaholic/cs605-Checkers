#!/usr/bin/env python3.6

# Jacob McKenna
# UAF CS605 AI
# Created 1/19/18

# Board class and object
import board
import sys
import time

# Create board object
b = board.Board()
# print('board size in bytes:', sys.getsizeof(b))

# Visual testing
# Ten moves
print()	

print('initial board')
print()
b.printBoard()
print()
print('initial board')

for i in range(100):
	print('move', i)
	b.moveGenerator()
	time.sleep(.8)
