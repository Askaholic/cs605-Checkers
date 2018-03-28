// move_table.h
// Rohan Weeden
// Created: March 27, 2018

// Declaration of move table

#ifndef MOVE_TABLE_H
#define MOVE_TABLE_H

#include <vector>


/******************************************************
 * ******************* MOVE TABLE ******************* *
 ******************************************************

 * The first index represents the starting location:
 *    moveTable[i] - i is a location on the board
 *
 * The second vector is the possible moves from that location
 *    moveTable[i][j] - j is the j'th possible location that is reachable from
 *                      location i (or -1 to pad the vector)
 *                    - The first two locations are always in SOUTH direction and
 *                      the second two locations are always in the NORTH direction
 */
extern std::vector<std::vector<int>> moveTable;


#endif
