///
/// file:
///	 	binary_land_stage.c
///
/// description:
/// 	implementation for a Binary Land stage, which includes
/// 	the location of the goal, brackets, spaces, and walls, and
///     movement of the brackets in accordance to the left bracket
///
/// author:
/// 	awallien
///
/// date:
/// 	12/29/2020
///

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "priority_queue.h"


struct binarylandstage_s {
	int nrows;
	int ncols;
	char **board;
	unsigned int **num_moves;
	int left_brkt_pos[2];
	int right_brkt_pos[2];
	int goal_pos[2];
};

typedef struct _bls_pq_node_t {
	int pos[2];
	int nhops;
}

#define ROW_POS( stage, which ) (stage -> which##_pos[0])
#define COL_POS( stage, which ) (stage -> which##_pos[1])
#define BOARD( stage, row, col ) (stage -> board[row][col])

#include "binary_land_stage.h"
#include "get_line.h"

#define EMPTY '.'
#define GOAL 'G'
#define LEFT_BRKT '['
#define RIGHT_BRKT ']'
#define WALL 'x'
#define NEWLINE '\n'

#define IS_NOT( stage, row, col, ch ) (BOARD( stage, row, col ) != ch)
#define IS_NOT_WALL( stage, row, col ) (BOARD( stage, row, col ) != WALL)


/// if condition met, print the error and, if any, free the objects
#define FAIL_COND_RET_NULL( cond, obj_to_free, line_to_free, error_msg, ... )	\
	if ( cond ) { 																	\
		fprintf( stderr, error_msg, __VA_ARGS__ );									\
		if ( obj_to_free != NULL ) dtor_BinaryLandStage( obj_to_free );				\
		if ( line_to_free != NULL ) free( line_to_free ); 							\
		return NULL; 																\
	}


/// parse number of rows and columns from the first line of config file
static int 
parse_number_of_rows_and_cols( char* line, int* nrows, int* ncols )
{
#define MAX_DIGITS 5
#define RET_FAIL_IF( cond )  				\
	if ( cond ) {			 				\
		return EXIT_FAILURE; 				\
	}
#define SKIP_WS while( line[i] != 0 && isspace( line[i] ) && ++i < 1024 );
#define READ( buffer ) \
	buffer_at = 0;								\
	for ( ; i < len_line; i++ ) { 				\
		if ( isspace( line[i] ) )				\
			break;								\
		RET_FAIL_IF( buffer_at >= MAX_DIGITS ); \
		if( line[i] >= '0' && line[i] <= '9' )	\
			buffer[buffer_at++] = line[i];		\
		else RET_FAIL_IF( 1 );					\
	}

	char buffer_rows[MAX_DIGITS+1] = { '\0' };
	char buffer_cols[MAX_DIGITS+1] = { '\0' };
	int buffer_at;
	int len_line = (int) strlen(line);
	int i = 0;

	SKIP_WS;

	READ( buffer_rows );
	
	SKIP_WS;

	READ( buffer_cols );

	SKIP_WS;

	RET_FAIL_IF( i != len_line );
	RET_FAIL_IF( !strlen( buffer_rows ) && !strlen( buffer_cols ) );

	sscanf( buffer_rows, "%d", nrows );
	sscanf( buffer_cols, "%d", ncols );

	return EXIT_SUCCESS;
}


/// BinaryLandStage constructor - allocates memory for a binary land stage
BinaryLandStage 
ctor_BinaryLandStage( FILE* file )
{
	BinaryLandStage s = (BinaryLandStage) calloc( 1, sizeof( struct binarylandstage_s ) );
	FAIL_COND_RET_NULL( !s, s, NULL,
		"Error: unable to allocate memory for BinaryLandStage%s\n", "");	

	s->left_brkt_pos[0] = -1;
	s->right_brkt_pos[0] = -1;
	s->goal_pos[0] = -1;

	char* line = NULL;
	size_t dummy;
	int read_line;

	read_line = get_line( &line, &dummy, file );
	FAIL_COND_RET_NULL( read_line < 0, s, line, 
		"Error: empty file%s\n", "" );

	int nrows = 0;
	int ncols = 0;

	// parse the number of rows and columns from the first line
	FAIL_COND_RET_NULL( 
		parse_number_of_rows_and_cols( line, &nrows, &ncols ), s, line,
		"Error: unable to parse dimensions from line: %s\n", line 
	);

	FAIL_COND_RET_NULL( nrows < 1, s, line,
		"Error: [%d] invalid number of rows.\n", nrows );

	FAIL_COND_RET_NULL( ncols < 3, s, line,
		"Error: [%d] invalid number of columns.\n", ncols );

	s->nrows = nrows;
	s->ncols = ncols;

	s->board = calloc( nrows, sizeof( char* ) );
	FAIL_COND_RET_NULL( !s->board, s, line,
		"Error: unable to allocate memory for stage of row size %d.\n", nrows);

	// find start positions, goal positions, and the whole board
	for(int r=0; r<nrows; r++){
		read_line = get_line( &line, &dummy, file );
		FAIL_COND_RET_NULL( read_line < 0, s, line, 
			"Error: [%d] empty row line\n", r );	

		s->board[r] = (char*) calloc( ncols, sizeof( char ) ); 
		FAIL_COND_RET_NULL( !s->board[r], s, line,
			"Error: unable to allocate memory for board row %d.\n", r );
		
		for( int c=0; c<ncols; c++ ) {
			char ch = line[c];
			switch( ch ) {
				case EMPTY:
					break;
				case GOAL:
					s->goal_pos[0] = r;
					s->goal_pos[1] = c;
					break;
				case LEFT_BRKT:
					s->left_brkt_pos[0] = r;
					s->left_brkt_pos[1] = c;
					break;
				case RIGHT_BRKT:
					s->right_brkt_pos[0] = r;
					s->right_brkt_pos[1] = c;
					break;
				case WALL:
					break;
				case NEWLINE:
					FAIL_COND_RET_NULL( c != ncols, s, line,
						"Error: line %d of size %d does not match given size %d\n", r+1, c, ncols ); 
					break;
				default:
					FAIL_COND_RET_NULL( 1, s, line, 
						"Error: invalid chr on line %d: %s\n", r+1, line);
			}
			s->board[r][c] = ch != NEWLINE && ch != LEFT_BRKT && ch != RIGHT_BRKT ? ch : EMPTY; 
		}

		free( line );	
		line = NULL;
	}

	s->num_moves = NULL;

	FAIL_COND_RET_NULL( s->left_brkt_pos[0] == -1, s, line, 
		"Error: left bracket position is not found%s\n", ""  );
	FAIL_COND_RET_NULL( s->right_brkt_pos[0] == -1, s, line,
		"Error: right bracket start position is not found%s\n", "" );
	FAIL_COND_RET_NULL( s->goal_pos[0] == -1, s, line, 
		"Error: goal position is not found%s\n", "" );

	return s;
}

/// debug print
// void 
// debug_BinaryLandStage( BinaryLandStage stage, FILE* out_file )
// {
// 	if ( stage ) {
// 		fprintf( out_file, "Size: %dx%d\nG: [%d,%d]\nStart\n  [: [%d,%d]\n  ]: [%d,%d]\n",
// 					stage->nrows, stage->ncols,
// 					stage->goal_pos[0], stage->goal_pos[1],
// 					stage->left_brkt_pos[0], stage->left_brkt_pos[1],
// 					stage->right_brkt_pos[0], stage->right_brkt_pos[1]
// 			   );

// 		int r, c;
// 		for( r=0; r<stage->nrows; r++ ) {
// 			char buffer[stage->ncols+2];
// 			for( c=0; c<stage->ncols; c++ )
// 				buffer[c] = stage->board[r][c];

// 			buffer[stage->ncols] = '\n';
// 			buffer[stage->ncols+1] = '\0';
// 			fputs( buffer, out_file );
// 		}
// 	}

// }

/// move the left and right brackets on the stage, dependent on the
/// movement of the left bracket, right bracket moves polar opposite
bool 
move_BinaryLandStage( BinaryLandStage stage, Direction dir ) 
{
	switch( dir ) {
		case dir_left:
		{
			int left_brkt_col = COL_POS( stage, left_brkt ) - 1;
			int right_brkt_col = COL_POS( stage, right_brkt ) + 1;

			if ( right_brkt_col < stage->ncols &&
					BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) != WALL ) {				 
				COL_POS( stage, right_brkt ) = right_brkt_col;
			}
	
			if ( left_brkt_col >= 0 &&
					BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) != WALL ) {
				COL_POS( stage, left_brkt ) = left_brkt_col;
			}

			break;
		}
		case dir_right:
		{
			int left_brkt_col = COL_POS( stage, left_brkt ) + 1;
			int right_brkt_col = COL_POS( stage, right_brkt ) - 1;

			if ( right_brkt_col >= 0 &&
					BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) != WALL ) {			 
				COL_POS( stage, right_brkt ) = right_brkt_col;
			}

			if ( left_brkt_col < stage->ncols &&
					BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) != WALL ) {
				COL_POS( stage, left_brkt ) = left_brkt_col;
			}

			break;
		}
		case dir_up: 
		{
			int left_brkt_row = ROW_POS( stage, left_brkt ) - 1;
			int right_brkt_row = ROW_POS( stage, right_brkt ) - 1;

			if ( right_brkt_row >= 0 &&
					BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) != WALL ) {
				ROW_POS( stage, right_brkt ) = right_brkt_row;
			}

			if ( left_brkt_row >= 0 && 
					BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) != WALL ) {
				ROW_POS( stage, left_brkt ) = left_brkt_row;
			}
			
			break;
		}
		case dir_down: 
		{
			int left_brkt_row = ROW_POS( stage, left_brkt ) + 1;
			int right_brkt_row = ROW_POS( stage, right_brkt ) + 1;

			if ( right_brkt_row < stage->nrows &&
					BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) != WALL ) {
				ROW_POS( stage, right_brkt ) = right_brkt_row;
			}

			if ( left_brkt_row < stage->nrows && 
					BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) != WALL ) {
				ROW_POS( stage, left_brkt ) = left_brkt_row;
			}

			break;
		}
		default:
			fprintf( stderr, "Unknown Direction value: %d\n", dir );
			return false;
	};

	return true;
} 

/// true if one of the brackets is able to change position; otherwise false
bool 
can_move_BinaryLandStage( BinaryLandStage stage, Direction dir )
{
	int pos_right_brkt, pos_left_brkt;
	switch( dir ) {
		case dir_left:
		{
			pos_right_brkt = COL_POS( stage, right_brkt ) + 1;
			pos_left_brkt = COL_POS( stage, left_brkt ) - 1;

			return ( pos_right_brkt < stage->ncols && IS_NOT_WALL( stage, ROW_POS( stage, right_brkt ), pos_right_brkt ) ) || 
					( pos_left_brkt >= 0 && IS_NOT_WALL( stage, ROW_POS( stage, left_brkt ), pos_left_brkt ) );
		}
		case dir_right:
		{
			pos_right_brkt = COL_POS( stage, right_brkt ) - 1;
			pos_left_brkt = COL_POS( stage, right_brkt ) + 1;

			return ( pos_right_brkt >= 0 && IS_NOT_WALL( stage, ROW_POS( stage, right_brkt ), pos_right_brkt ) ) ||
					( pos_left_brkt < stage->ncols && IS_NOT_WALL( stage, ROW_POS( stage, left_brkt ), pos_left_brkt ) );
		}
		case dir_up:
		{
			pos_right_brkt = ROW_POS( stage, right_brkt ) - 1;
			pos_left_brkt = ROW_POS( stage, left_brkt ) - 1;

			return ( pos_right_brkt >= 0 && IS_NOT_WALL( stage, pos_right_brkt, COL_POS( stage, right_brkt ) ) ) ||
					( pos_left_brkt >= 0 && IS_NOT_WALL( stage, pos_left_brkt, COL_POS( stage, left_brkt ) ) ); 
		}
		case dir_down:
		{
			pos_right_brkt = ROW_POS( stage, right_brkt ) + 1;
			pos_left_brkt = ROW_POS( stage, left_brkt ) + 1;

			return ( pos_right_brkt < stage->nrows && IS_NOT_WALL( stage, pos_right_brkt, COL_POS( stage, right_brkt ) ) ) ||
					( pos_left_brkt < stage->nrows && IS_NOT_WALL( stage, pos_left_brkt, COL_POS( stage, left_brkt ) ) ); 
		}
		default:
			fprintf( stderr, "Unknown Direction value: %d\n", dir );
			return false;
	}
}

bool 
reverse_move_BinaryLandStage( BinaryLandStage stage, Direction dir )
{
	switch( dir ) {
		case dir_left:
			return move_BinaryLandStage( stage, dir_right );
		case dir_right:
			return move_BinaryLandStage( stage, dir_left );
		case dir_up:
			return move_BinaryLandStage( stage, dir_down );
		case dir_down:
			return move_BinaryLandStage( stage, dir_up );
		default:
			fprintf( stderr, "Unknown Direction value: %d\n", dir );
			return false;
	}
}

/// get the board and assign values to pointers
char**
board_BinaryLandStage( BinaryLandStage stage, int* nrows_ptr, int* ncols_ptr )
{
	*nrows_ptr = stage->nrows;
	*ncols_ptr = stage->ncols;
	return stage->board;
}

char
char_BinaryLandStage( Character c )
{
	switch(c) {
		case char_left_brkt:
			return LEFT_BRKT;
		case char_right_brkt:
			return RIGHT_BRKT;
		case char_goal:
			return GOAL;
		default:
			fprintf( stderr, "Invalid Character value: %d", c );
			return EMPTY;
	}
}

/// get position of a character
void 
pos_BinaryLandStage( BinaryLandStage stage, Character chr, int* row_pos, int* col_pos )
{
	switch( chr ) {
		case char_left_brkt:
			*row_pos = ROW_POS( stage, left_brkt );
			*col_pos = COL_POS( stage, left_brkt );
			break;
		case char_right_brkt:
			*row_pos = ROW_POS( stage, right_brkt );
			*col_pos = COL_POS( stage, right_brkt );
			break;
		case char_goal:
			*row_pos = ROW_POS( stage, goal );
			*col_pos = COL_POS( stage, goal );
			break;
		default:
			fprintf( stderr, "Unknown Character value: %d\n", chr );
			*row_pos = -1;
			*col_pos = -1;
	}
}

/// did the brackets reach the goal?
bool
is_solved_BinaryLandStage( BinaryLandStage stage )
{
	return  ROW_POS( stage, left_brkt ) == ROW_POS( stage, goal ) && 
			ROW_POS( stage, goal ) == ROW_POS( stage, right_brkt ) &&
			COL_POS( stage, left_brkt ) == COL_POS( stage, goal ) - 1 && 
			COL_POS( stage, right_brkt ) == COL_POS( stage, goal ) + 1;
}

static void
get_adj_tiles( BinaryLandStage stage, int pos[2], int adj_tiles[4][2], int *num_adj_tiles)
{
#define IS_VALID_POS( r, c ) (r >= 0 && c >=0 && r < stage->nrows && c < stage->ncols )
	
	int r_pos = pos[0];
	int c_pos = pos[1];
	int r_idx, c_idx;
	int r, c;
	int adj_cnt = 0;

	for ( r_idx = -1; r_idx <= 1; r_idx+=2 )
		for ( c_idx = -1; c_idx <= 1; c_idx+=2 )
			r = r_pos + r_idx;
			c = c_pos + c_idx;
			if ( IS_VALID_POS( r, c ) && 
			     IS_NOT_WALL( stage, r, c ) && 
				 IS_NOT( stage, r, c, GOAL ) ) {
				adj_tiles[adj_cnt][0] = r;
				adj_tiles[adj_cnt][1] = c;
				++adj_cnt;
			}

	*num_adj_tiles = adj_cnt;
}

/// calculate the number of moves/hop for each white space in board
///  @pre stage->num_moves should NOT be NULL
static void
calc_moves( BinaryLandStage stage )
{
	PriorityQueue pq = NULL;
	int 	*goal_pos,
			 nrows,
			 ncols;
	int		 r_idx, c_idx;
	u_int  **num_moves;
	char   **board;
	int    adj_tiles[4][2];
	int    num_adj_tiles = 0;
	int    hops = 0;
	
	goal_pos = stage->goal_pos;
	nrows = stage->nrows;
	ncols = stage->ncols;
	num_moves = stage->num_moves;
	board = stage->board;
	pq = ctor_PriorityQueue();

	/* Scan board first for any walls */
	for ( r_idx = 0; r_idx < nrows; r_idx++ ) {
		for ( c_idx = 0; c_idx < ncols; c_idx++ ) {
			if ( !IS_NOT_WALL(stage, r_idx, c_idx) ) {
				num_moves[r_idx][c_idx] = UINT_MAX;
			}
		}
	}

	get_adj_tiles( stage, goal_pos, adj_tiles, &num_adj_tiles );

	for ( r_idx = 0; r_idx < num_adj_tiles; r_idx++ ) {
		
	}

	dtor_PriorityQueue( pq );
}


/// allocate moves and calculate the number of moves for each white space to goal
u_int **
ctor_moves_BinaryLandStage( BinaryLandStage stage )
{
	FAIL_COND_RET_NULL( !stage, NULL, NULL, "Error: stage is null%s\n", "" );

	int r_idx = 0;

	if ( stage->num_moves != NULL )
		return stage->num_moves;

	stage->num_moves = calloc( stage->nrows, sizeof( u_int * ) );
	FAIL_COND_RET_NULL( !stage->num_moves, NULL, NULL, 
						"Error: Unable to allocate memory for stage of num moves%s\n",
						"" );
	
	for ( r_idx = 0; r_idx < stage->nrows; r_idx++ ) {
		stage->num_moves[r_idx] = calloc( stage->ncols, sizeof( u_int ) );
		FAIL_COND_RET_NULL( !stage->num_moves[r_idx], NULL, NULL, 
							"Error: Unable to allocate memory for stage of num moves%s\n",
							"" );
	}

	calc_moves( stage );

	return stage->num_moves;
}


/// deallocating the board's stage by row
static void 
free_board( char** board, int nrows )
{
	for(int r=0; r<nrows; r++){
		if(board[r])
			free(board[r]);
	}
	free(board);
	board = NULL;
}

/// deallocating the board's num moves
static void
free_moves_board( u_int** board, int nrows )
{
	for(int r=0; r<nrows; r++){
		if(board[r])
			free(board[r]);
	}
	free(board);
	board = NULL;
}


/// deallocate BinaryLandStage object
void 
dtor_BinaryLandStage( BinaryLandStage stage )
{
	if( stage ) {
		if ( stage->board )
			free_board( stage->board, stage->nrows );
		if ( stage->num_moves )
			free_moves_board( stage->num_moves, stage->nrows );
		free( stage );
	}
    stage = NULL;
}
