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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct binarylandstage_s {
	int nrows;
	int ncols;
	char** board;
	int left_brkt_pos[2];
	int right_brkt_pos[2];
	int goal_pos[2];
};

#define ROW_POS( stage, which ) stage-> which##_pos[0]
#define COL_POS( stage, which ) stage-> which##_pos[1]
#define BOARD( stage, row, col ) stage-> board[row][col]

#include "binary_land_stage.h"
#include "get_line.h"

#define EMPTY '.'
#define GOAL 'G'
#define LEFT_BRKT '['
#define RIGHT_BRKT ']'
#define WALL 'x'
#define NEWLINE '\n'


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
		read_line = get_line(&line, &dummy, file);
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
			if( ch != NEWLINE )
				s->board[r][c] = ch; 
		}

		free( line );	
		line = NULL;
	}

	FAIL_COND_RET_NULL( s->left_brkt_pos[0] == -1, s, line, 
		"Error: left bracket position is not found%s\n", ""  );
	FAIL_COND_RET_NULL( s->right_brkt_pos[0] == -1, s, line,
		"Error: right bracket start position is not found%s\n", "" );
	FAIL_COND_RET_NULL( s->goal_pos[0] == -1, s, line, 
		"Error: goal position is not found%s\n", "" );

	return s;
}

/// debug print
void 
debug_BinaryLandStage( BinaryLandStage stage, FILE* out_file )
{
	if ( stage ) {
		fprintf( out_file, "Size: %dx%d\nG: [%d,%d]\nStart\n  [: [%d,%d]\n  ]: [%d,%d]\n",
					stage->nrows, stage->ncols,
					stage->goal_pos[0], stage->goal_pos[1],
					stage->left_brkt_pos[0], stage->left_brkt_pos[1],
					stage->right_brkt_pos[0], stage->right_brkt_pos[1]
			   );

		int r, c;
		for( r=0; r<stage->nrows; r++ ) {
			char buffer[stage->ncols+2];
			for( c=0; c<stage->ncols; c++ )
				buffer[c] = stage->board[r][c];
			buffer[stage->ncols] = '\n';
			buffer[stage->ncols+1] = '\0';
			fputs( buffer, out_file );
		}
	}

}

static bool
adjacent_BinaryLandStage( BinaryLandStage stage )
{
	return ROW_POS( stage, left_brkt ) == ROW_POS( stage, right_brkt ) &&
			abs( COL_POS( stage, left_brkt ) - COL_POS( stage, right_brkt ) ) == 1;
}


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
				BOARD( stage, ROW_POS( stage, right_brkt ), COL_POS( stage, right_brkt )) = EMPTY;
				BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) = RIGHT_BRKT;
				COL_POS( stage, right_brkt ) = right_brkt_col;
			}
	
			if ( left_brkt_col >= 0 &&
					BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) != WALL ) {
				BOARD( stage, ROW_POS( stage, left_brkt ), COL_POS( stage, left_brkt ) ) = EMPTY;
				BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) = LEFT_BRKT;
				COL_POS( stage, left_brkt ) = left_brkt_col;
			}

			if ( adjacent_BinaryLandStage( stage ) ) {
				BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) = LEFT_BRKT;
				BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) = RIGHT_BRKT;
			}

			break;
		}
		case dir_right:
		{
			int left_brkt_col = COL_POS( stage, left_brkt ) + 1;
			int right_brkt_col = COL_POS( stage, right_brkt) - 1;

			if ( right_brkt_col >= 0 &&
					BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) != WALL ) {
				BOARD( stage, ROW_POS( stage, right_brkt ), COL_POS( stage, right_brkt ) ) = EMPTY;	
				BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) = RIGHT_BRKT;			 
				COL_POS( stage, right_brkt ) = right_brkt_col;
			}

			if ( left_brkt_col < stage->ncols &&
					BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) != WALL ) {
				BOARD( stage, ROW_POS( stage, left_brkt ), COL_POS( stage, left_brkt ) ) = EMPTY;
				BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) = LEFT_BRKT;
				COL_POS( stage, left_brkt ) = left_brkt_col;
			}

			if ( adjacent_BinaryLandStage( stage ) ) {
				BOARD( stage, ROW_POS( stage, left_brkt ), left_brkt_col ) = LEFT_BRKT;
				BOARD( stage, ROW_POS( stage, right_brkt ), right_brkt_col ) = RIGHT_BRKT;
			}

			break;
		}
		case dir_up: 
		{
			int left_brkt_row = ROW_POS( stage, left_brkt ) - 1;
			int right_brkt_row = ROW_POS( stage, right_brkt ) - 1;

			if ( right_brkt_row >= 0 &&
					BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) != WALL ) {
				BOARD( stage, ROW_POS( stage, right_brkt ), COL_POS( stage, right_brkt ) ) = EMPTY;
				BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) = RIGHT_BRKT;
				ROW_POS( stage, right_brkt ) = right_brkt_row;
			}

			if ( left_brkt_row >= 0 && 
					BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) != WALL ) {
				BOARD( stage, ROW_POS( stage, left_brkt ), COL_POS( stage, left_brkt ) ) = EMPTY;
				BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) = LEFT_BRKT;
				ROW_POS( stage, left_brkt ) = left_brkt_row;
			}

			if ( adjacent_BinaryLandStage( stage ) ) {
				BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) = LEFT_BRKT;
				BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) = RIGHT_BRKT;
			}
			
			break;
		}
		case dir_down: 
		{
			int left_brkt_row = ROW_POS( stage, left_brkt ) + 1;
			int right_brkt_row = ROW_POS( stage, right_brkt ) + 1;

			if ( right_brkt_row < stage->nrows &&
					BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) != WALL ) {
				BOARD( stage, ROW_POS( stage, right_brkt ), COL_POS( stage, right_brkt ) ) = EMPTY;
				BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) = RIGHT_BRKT;
				ROW_POS( stage, right_brkt ) = right_brkt_row;
			}

			if ( left_brkt_row < stage->nrows && 
					BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) != WALL ) {
				BOARD( stage, ROW_POS( stage, left_brkt ), COL_POS( stage, left_brkt ) ) = EMPTY;
				BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) = LEFT_BRKT;
				ROW_POS( stage, left_brkt ) = left_brkt_row;
			}

			if ( adjacent_BinaryLandStage( stage ) ) {
				BOARD( stage, left_brkt_row, COL_POS( stage, left_brkt ) ) = LEFT_BRKT;
				BOARD( stage, right_brkt_row, COL_POS( stage, right_brkt ) ) = RIGHT_BRKT;
			}
			
			break;
		}
		default:
			fprintf( stderr, "Unknown Direction value: %d\n", dir );
			return false;
	};

	// precedence of what char appears at the goal position: left brkt, right brkt, GOAL
	if ( ROW_POS( stage, goal ) == ROW_POS( stage, left_brkt ) && COL_POS( stage, goal ) == COL_POS( stage, left_brkt ) ) {
		BOARD( stage, ROW_POS( stage, goal ), COL_POS( stage, goal ) ) = LEFT_BRKT;
	} else if ( ROW_POS( stage, goal ) == ROW_POS( stage, right_brkt ) && COL_POS( stage, goal ) == COL_POS( stage, right_brkt ) ) {
		BOARD( stage, ROW_POS( stage, goal ), COL_POS( stage, goal ) ) = RIGHT_BRKT;
	} else {
		BOARD( stage, ROW_POS( stage, goal ), COL_POS( stage, goal ) ) = GOAL;
	}

	return true;
} 

/// get the board
char**
board_BinaryLandStage( BinaryLandStage stage, int* nrows_ptr, int* ncols_ptr )
{
	*nrows_ptr = stage->nrows;
	*ncols_ptr = stage->ncols;
	return stage->board;
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


/// deallocate BinaryLandStage object
void 
dtor_BinaryLandStage( BinaryLandStage stage )
{
	if(stage) {
		if (stage->board)
			free_board(stage->board, stage->nrows);
		free(stage);
	}
    stage = NULL;
}