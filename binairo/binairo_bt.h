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
/// bt_initialize
///
/// initialize the backtracker for binairo
/// THIS MUST BE CALLED BEFORE CALLING THE solve FUNCTION
///
/// @param board - the starting binairo board
/// @param debug - is debug enabled?
/// 
void bt_initialize( BinairoBoard board, bool debug );


///
/// solve
///
/// the main function to initialize, apply
/// heuristics, and run the backtracking
/// algorithm
///
/// @notice: if bt_initialize is not called before this function,
/// 			the function will return false
///
/// @return true if solution is found; otherwise, false
///
bool solve( );



#endif //__BINAIRO_BT_H_

