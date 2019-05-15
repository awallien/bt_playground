///
/// file: 
///		binairo_bt.c
///
/// author:
///		awallien
///
/// description: 
///		"class" that contains the implementation
///		of a Binairo configuration backtracker
///
/// date:
///		5/12/19
///


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "binairo_bt.h"


/// digits to put on the board for solving and validating
static char digits[2] = {'0','1'};

/// the binairo board to be solved
static BinairoBoard brd;

///
/// is_goal
///
/// the goal is reached if the board is
/// absolutely filled up and everything
/// is valid
/// 
/// @param area the area of the board
/// @param status the current cell on the board
///
/// @return current cell on board is last cell on board
///
static bool is_goal( size_t area, size_t status ) { return status == area; }


///
/// is_valid
///
/// when a piece is put on a cell, it would check the following:
/// 	if the piece is not at the end of the row:
///			- check adjacent digits
///			- number of 0s == number of 1s
///			- 
///
/// @param spot - the cell spot that is being validated
///
/// @return true if the digit at cell is valid; otherwise, false
///
static bool is_valid( int status ) { return false; }


///
/// bt_solve
///
/// performs the actual backtracking method
///
/// @param status the cell we are currently backtracking
///
/// @return true if backtracker finds a solution; otherwise, false
///
static bool bt_solve( int status ) {
	
	// goal reached
	if( is_goal( dim_BinairoBoard( brd )*dim_BinairoBoard( brd ), status ) )
		return true;
	
	// no solution found
	if( status < 0 )
		return false;
	
	// check if cell in board is already marked
	if( is_marked_BinairoBoard( brd, status ) )
		return bt_solve( status+1 );
	
	// lay digits and validate
	else{
		for( int i=0; i<2; i++ ){

			// put digit in spot
			put_BinairoBoard( brd, status, digits[i] );

			// advance a depth if valid		
			if( is_valid( status ) )
					return bt_solve( status+1 );

		}
	
		// invalid check, erase digit and backtrack
		erase_BinairoBoard( brd, status );
		return bt_solve( status-1 );
	}

}


/// the "main" function for this backtracking 
bool solve( BinairoBoard b ) {

	// initialize board
	brd = b;

	// beginning at the starting cell
	return bt_solve( 0 );

}


