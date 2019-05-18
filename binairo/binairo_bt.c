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


/// the binairo board to be solved
static BinairoBoard brd;
static int dim;

///
/// is_goal
///
/// the goal is reached if the board is
/// absolutely filled up and everything
/// is valid
/// 
/// @param area - the area of the board
/// @param status - the current cell on the board
///
/// @return - current cell on board is last cell on board
///
static bool is_goal( int area, int status ) { return status == area; }


///
/// chk_[left,right,up,down,mid]_adj
///
/// list of functions that validates a digit being
/// put in a cell on the board by checking the adjacent
/// cells
///
/// @param status - the cell being validated
/// @param digit - the digit in the cell
///
/// @return - true if digit can be put in that cell; otherwise, false
///
static bool chk_left_adj( int status, Digit digit ){
	char idx = 2;
	char count = 0;
	while( status > 0 && (--status)%(int)dim >= 0 && idx-- )
		count += get_BinairoBoard( brd, status ) == digit ? 1 : 0;
	return count != 2; 
}

static bool chk_right_adj( int status, Digit digit ){
	char idx = 2;
	char count = 0;
	while( (++status)%dim != 0 && idx-- )
		count += get_BinairoBoard( brd, status ) == digit ? 1 : 0;
	return count != 2;
}

static bool chk_up_adj( int status, Digit digit ){
	char idx = 2;
	char count = 0;
	while( (status-=dim) > 0 && idx-- )
		count += get_BinairoBoard( brd, status ) == digit ? 1 : 0;  
	return count != 2;
}

static bool chk_down_adj( int status, Digit digit ){
	char idx = 2;
	char count = 0;
	while( (status+=dim) < dim*dim && idx-- )
		count += get_BinairoBoard( brd, status ) == digit ? 1 : 0;
	return count != 2;	
}

static bool chk_mid_adj( int status, Digit digit ){
	char count = 0;
	int cur = status % dim;
	int left = (status-1)%dim;
	int right = (status+1)%dim;
	if( left > 0 && left < cur && get_BinairoBoard( brd, left ) == digit )
		count++;
	if( right > cur && get_BinairoBoard( brd, right ) == digit )
		count++;
	return count != 2;
}


///
/// is_valid
///
/// when a digit is put on a cell, it would check the following:
/// 	if the piece is not at the end of the row:
/// 		- check adjacent digits (horizontal and vertical)
///			- number of 0s == number of 1s
///		if cell spot is at end of row:
///			- check for unique row
///		if cell spot is at end of column:
///			- check for unique column
///
/// @param status - the cell spot that is being validated
///
/// @return true if the digit at cell is valid; otherwise, false
///
static bool is_valid( int status ) { 

	Digit d = get_BinairoBoard( brd, status );
	
	bool chk = chk_left_adj( status, d ) && 
		chk_right_adj( status, d ) && 
		chk_up_adj( status, d ) && 
		chk_down_adj( status, d ) && 
		chk_mid_adj( status, d );

	return chk;
	
}

/// TODO: for going forward and backward correctly, works for now
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
	if( is_goal( dim*dim, status ) )
		return true;
	
	// no solution found
	if( status < 0 )
		return false;
	
	// check if cell in board is already marked
	if( is_marked_BinairoBoard( brd, status ) )
		return bt_solve( status+1 );
	
	// lay digits and validate
	else{
		for( Digit i=ZERO; i<=ONE; i++ ){

			// put digit in spot
			put_BinairoBoard( brd, status, i );

			// advance a depth if valid		
			if( is_valid( status ) && bt_solve( status+1 ) )
					return true;

			// invalid check, erase digit and backtrack
			put_BinairoBoard( brd, status, BLANK );
		}
	
		return false;
	}
}


///
/// apply_heuristics
///
/// will be implemented later
///
static void apply_heuristics( ){}


/// the "main" function for this backtracking 
bool solve( BinairoBoard b ) {
	// initialize board
	brd = b;
	dim = dim_BinairoBoard( b );

	// apply heuristics on board
	apply_heuristics( );

	// beginning at the starting cell
	return bt_solve( 0 );
}


