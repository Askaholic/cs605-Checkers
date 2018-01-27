#!/usr/bin/env python3.6

# Jacob McKenna
# UAF CS605 AI
# Created 1/19/18

# Board class and object
import board
import sys
import os
import time

b = board.Board()
# print('board size in bytes:', sys.getsizeof(b))


print()	

print('initial board')
print()
b.printBoard()
print()
print('initial board')
time.sleep(1.8)
os.system('clear')


for i in range(100):
	print('move', i)
	if (b.moveGenerator()):
		break
	time.sleep(3)
	os.system('clear')
