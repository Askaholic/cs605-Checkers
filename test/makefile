test: test.o board_funcs.o
	g++ -o test test.o

board_funcs.o:
	g++ -c --std=c++14 ../src/board_funcs/board_funcs.cpp

test.o:
	g++ -c --std=c++14 test.cpp
