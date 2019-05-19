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
/// chk_unique_[rows,cols]
///
/// checks if each row or column is unique to the other rows or columns
/// previous to the current
///
/// @param status - the current cell spot
///
/// @return - true if all rows and columns are unique; otherwise, false
///
/// @pre - status should be a spot at an end of a row or a column 
static bool chk_unique_rows( int status ){

	if( status%dim != dim-1 )
		return true;

	int idx = status/dim;
	while( idx-- > 0 ){
					
	}
	return true;
	
}

static bool chk_unique_cols( int status ){
	return false;
}


///
/// is_valid
///
/// when a digit is put on a cell, it would check the following:
/// 	if the piece is not at the end of the row:
/// 		- check adjacent digits (horizontal and vertical)
///			- number of 0s == number of 1s in row and col
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

	// check number of 0s == number of 1s in row
	if( numberof_BinairoBoard( brd, status/dim, ZERO ) > dim/2 || 
			numberof_BinairoBoard( brd, status/dim, ONE ) > dim/2 )
		return false;	

	// check adjacency	
	if( !(	chk_left_adj( status, d ) && 
		chk_right_adj( status, d ) && 
		chk_up_adj( status, d ) && 
		chk_down_adj( status, d ) && 
		chk_mid_adj( status, d ) )
	)
		return false;

	// at end of row
	if( !chk_unique_rows( status ) )
		return false;	

	return true;	
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


