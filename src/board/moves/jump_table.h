// jump_table.h
// Rohan Weeden
// Created: Feb. 22, 2018

// Header file for holding the monstrosity that is the jump table

#ifndef BF_JUMP_TABLE
#define BF_JUMP_TABLE


#include <vector>

/******************************************************
 * ******************* JUMP TABLE ******************* *
 ******************************************************

 * The first index represents the starting location:
 *    jumpTable[i] - i is a location on the board
 *
 * The second vector is the possible jumps from that location
 *    jumpTable[i][j] - j is the j'th possible jump from location i
 *
 * The third vector is a 'to' location, and an 'enemy' location, where enemy is
 * the square we would be jumping over.
 *    jumpTable[i][j][0] - where the checker will end up after the jump
 *    jumpTable[i][j][1] - the piece that the checker will jump over
 */
extern std::vector<std::vector<std::vector<int>>> jumpTable;

#endif
