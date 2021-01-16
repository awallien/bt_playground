///
/// file:
/// 	binary_land_solve.h
///
///	description:
///		header file containing method to solve BinaryLandStage 
///
/// author:
///		awallien
///
///	date:
///		9/26/2020
///

#ifndef __BINARY_LAND_SOLVE_H_
#define __BINARY_LAND_SOLVE_H_

#include <stdbool.h>

#include "binary_land_stage.h"

///
/// init_solve_BinaryLand
///
/// initialize the solvers
///
/// @param stage the Binary Land Stage
///
/// @param delay if animated, the delay between frames
///
///
void init_solve_BinaryLand( BinaryLandStage stage, double delay ); 


///
/// solve Binary Land stage based on the given mode where
/// 	A - solve stage using A* search 
///		B - solve stage using DFS backtracking
///     C - solve stage using manual controls
///
/// @param stage the Binary Land stage
///
/// @param mode the mode used to solve this stage
///
/// @return false if not solvable; otherwise, true
///
bool solve_BinaryLand( char mode );

#endif
