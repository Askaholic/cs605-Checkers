// search_benchmark.cpp
// Rohan Weeden
// Created: April 5, 2018

// Used to generate stats on the minimax search report

#include "asknet4.h"
#include "board_funcs.h"
#include "player.h"
#include "search.h"
#include <chrono>
#include <functional>
#include <iostream>


int main(int argc, char const *argv[]) {
    BoardState board1 = make_board({
          'r','r','r','r',
        ' ','r','r','r',
          'r','r','r','r',
        'r',' ',' ',' ',
          ' ',' ',' ','b',
        'b','b','b',' ',
          'b','b','b','b',
        'b','b','b','b',
    });

    BoardState board2 = make_board({
          ' ',' ','r','r',
        ' ','r',' ',' ',
          'r','r','r',' ',
        'r','r','r',' ',
          ' ',' ',' ','r',
        'b','b',' ','b',
          'b','b','b','b',
        'b','b',' ',' ',
    });

    BoardState board3 = make_board({
          ' ',' ',' ','r',
        ' ','r','r',' ',
          'r','r',' ',' ',
        'r',' ','r','r',
          ' ',' ',' ',' ',
        'b','b','b',' ',
          'b',' ','r','b',
        'b',' ',' ','R',
    });

    BoardState board4 = make_board({
          ' ',' ',' ',' ',
        ' ',' ','r','r',
          'r',' ',' ',' ',
        ' ',' ',' ','r',
          ' ','b','r',' ',
        ' ',' ',' ',' ',
          ' ','r','r','b',
        'b',' ',' ','R',
    });

    BoardState board5 = make_board({
          ' ',' ',' ',' ',
        ' ',' ',' ',' ',
          ' ',' ',' ',' ',
        ' ',' ',' ','r',
          ' ',' ',' ','r',
        ' ','r',' ',' ',
          ' ',' ','r','R',
        'K','R','R',' ',
    });
    Network4 net({32, 40, 10, 1});
    net.randomizeWeights();

    AIPlayer player(RED_PLAYER, net);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = min_max_search(board1, RED_PLAYER, 8, std::bind(&AIPlayer::evaluate, player, std::placeholders::_1, std::placeholders::_2));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = (end - start);
    std::cout << "Recursive search took " << elapsed.count() << " seconds\n";


    std::cout << "Result from search: " << '\n';
    print_board(result.first);
    std::cout << "Score: " << result.second << '\n';
}
