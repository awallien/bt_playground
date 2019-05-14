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

#include "binairo_board.c"	// BinairoBoard


static bool is_valid( ) { return false; }

static void apply_value() { }

static void remove_value() { }

static bool bt_solve( int status ) {



}


/// the "main" function for this backtracking 
bool solve( BinairoBoard brd ) {

	return bt_solve( brd, 0 );

} 
