///
/// file: 
/// 		binairo_bt.h
///
/// author:
///		awallien
///
/// description:
///		given a binairo board, it attempts to solve
///		the puzzle from a heuristic and backtracking
///		approach
///
/// date:
///		5/14/19
///


#ifndef __BINAIRO_BT_H_
#define __BINAIRO_BT_H_

#include <stdbool.h>
#include "binairo_board.h"

///
/// solve
///
/// the main function to initialize, apply
/// heuristics, and run the backtracking
/// algorithm
///
/// @param brd the initial binairo board
///
/// @return true if solution is found; otherwise, false
///
bool solve( BinairoBoard brd );


#endif //__BINAIRO_BT_H_

