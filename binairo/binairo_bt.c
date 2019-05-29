///
/// file: 
///     binairo_bt.c
///
/// author:
///     awallien
///
/// description: 
///     "class" that contains the implementation
///     of a Binairo configuration backtracker
///
/// date:
///     5/12/19
///


#define _DEFAULT_SOURCE
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "binairo_board.h"
#include "binairo_bt.h"
#include "display.h"
#include "hash_info.h"

#define BLANK printf( "       " )
#define DEBUG_TRUE if( debug ) { set_cur_pos( 2*dim+2, 1 ); BLANK; puts( "\rVALID" ); usleep( delay ); }
#define DEBUG_FALSE if ( debug ) { set_cur_pos( 2*dim+2, 1 ); BLANK; puts( "\rINVALID" ); usleep( delay ); }
#define DEBUG_BRD if( debug ) { set_cur_pos( 1, 1 ); print_BinairoBoard( brd, stdout ); }


/// the binairo board to be solved
static BinairoBoard brd = NULL;

/// dimension of the board
static int dim = 0;

/// graphics debugging
static bool debug = false;

/// hash info for checking duplicate rows and columns
static HashInfo hashinfo = NULL;

/// delay for printing board in debug mode
static double delay = 1000000;


// initialize the backtracker
void bt_initialize( BinairoBoard b, bool d, double del ){
    brd = b;
    dim = dim_BinairoBoard( brd );
    debug = d;
	delay = del*1000000;
	hashinfo = create_HashInfo( dim );
}


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
/// static bool is_goal( int area, int status ) { return status == area; }

/// simpler line to check for goal
#define IS_GOAL dim*dim == status


///
/// chk_[left,right,up,down,midrow,midcol]_adj
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
    int cur = status%dim;
    while( --status >= 0 && status%dim <= cur && idx-- )
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
    while( (status-=dim) >= 0 && idx-- )
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

static bool chk_midrow_adj( int status, Digit digit ){
    char count = 0;
    int left = status-1;
    int right = status+1;
    if( left >= 0 && left%dim < status%dim && get_BinairoBoard( brd, left ) == digit )
        count++;
    if( right < dim*dim && right%dim > status%dim && get_BinairoBoard( brd, right ) == digit )
        count++;
    return count != 2;
}

static bool chk_midcol_adj( int status, Digit digit ){
    char count = 0;
    int top = status-dim;
    int bottom = status+dim;
    if( top >= 0 && get_BinairoBoard( brd, top ) == digit )
        count++;
    if( bottom < dim*dim && get_BinairoBoard( brd, bottom ) == digit )
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
///
static bool chk_unique_rows( int status ){
	// check if at end of row
	if( status%dim != dim-1 )
		return true;

	// build string to put in hash_info
	char* row_str = calloc( dim+1, sizeof( char ) );
	int idx;
	for( idx=0; idx<dim; idx++ ){
		switch( get_BinairoBoard( brd, status-dim+1+idx ) ){
			case ZERO:
				row_str[idx] = '0';
				break;
			case ONE:
				row_str[idx] = '1';
				break;
			default:
				assert( get_BinairoBoard( brd, status-dim+1+idx ) );
		}	
	}
	row_str[idx] = '\0';
	// put hash into collection and compare hash to others
	put_HashInfo( hashinfo, row_str, ROW, status/dim );
	free( row_str );
 	
	size_t cur_hash = get_HashInfo( hashinfo, ROW, status/dim );	
	while( (status-=dim) > 0 ){
		if( cur_hash == get_HashInfo( hashinfo, ROW, status/dim ) ){
			return false;	
		}
	}
	return true;
}

/*
static bool chk_unique_cols( int status ){
    return false;
}
*/

///
/// is_valid
///
/// when a digit is put on a cell, it would check the following:
///     if the piece is not at the end of the row:
///         - check adjacent digits (horizontal and vertical)
///         - number of 0s == number of 1s in row and col
///     if cell spot is at end of row:
///         - check for unique row
///     if cell spot is at end of column:
///         - check for unique column
///
/// @param status - the cell spot that is being validated
///
/// @return true if the digit at cell is valid; otherwise, false
///
static bool is_valid( int status ) { 
    Digit d = get_BinairoBoard( brd, status );

    // check number of 0s == number of 1s in row
    if( numberof_row_BinairoBoard( brd, status/dim, ZERO ) > dim/2 || 
            numberof_row_BinairoBoard( brd, status/dim, ONE ) > dim/2 ){
        DEBUG_FALSE;
        return false;
    }

    if( numberof_column_BinairoBoard( brd, status%dim, ZERO ) > dim/2 ||
            numberof_column_BinairoBoard( brd, status%dim, ONE ) > dim/2 ){
		DEBUG_FALSE;
		return false;
    }

    // check adjacency  
    if( !(  chk_left_adj( status, d ) && 
        chk_right_adj( status, d ) && 
        chk_up_adj( status, d ) && 
        chk_down_adj( status, d ) && 
        chk_midrow_adj( status, d )  &&
        chk_midcol_adj( status, d ) )
    ){
        DEBUG_FALSE;
        return false;
    }

    // at end of row
    if( !chk_unique_rows( status ) ){
        DEBUG_FALSE;
        return false;
    }   

    DEBUG_TRUE;
    return true;    
}


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
    if( IS_GOAL )
        return true;
    
    // no solution found
    if( status < 0 )
        return false;
    
    // check if cell in board is already marked
    if( is_marked_BinairoBoard( brd, status ) && chk_unique_rows( status ) )
        return bt_solve( status+1 );
    
    // lay digits and validate
    else{
        Digit i;
    	for( i=ZERO; i<=ONE; i++ ){

            // put digit in spot
            put_BinairoBoard( brd, status, i );

            DEBUG_BRD;

            // advance a depth if valid     
            if( is_valid( status ) && bt_solve( status+1 ) )
                    return true;

        }
       
        put_BinairoBoard( brd, status, i ); 
        DEBUG_BRD;
        return false;
    }
}


///
/// apply_heuristics
///
/// will be implemented later if I ever get back to this solver
///
static void apply_heuristics( ){}


/// the "main" function for this backtracking 
bool solve( ) {
    if( brd == NULL ){
        fprintf( stderr, "Error: board has not been initialized for backtracker.\n" );
        return false;
    }

    // apply heuristics on board
    apply_heuristics( );

    // beginning at the starting cell
    bool res = bt_solve( 0 );

	// done with the hash collection
	destroy_HashInfo( hashinfo );

	return res;
}


