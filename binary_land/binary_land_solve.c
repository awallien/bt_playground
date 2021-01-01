///
/// file:
///		binary_land_solve.c
///
/// description:
///		contains implementation of backtracker and A* searcher algorithms and manual interaction
///		to solve binary land stage
///
///	author:
///		awallien
///
/// date:
///		9/26/2020
///

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "binary_land_solve.h"
#include "binary_land_stage.h"

#define CARRIAGE_RETURN '\r'
#define NEWLINE '\n'
#define KEY_ESC 27

static BinaryLandStage stage = NULL;

static bool animate = false;

static long delay = 0;

void 
init_solve_BinaryLand( BinaryLandStage s, bool a, long d )
{
	stage = s;
	animate = a;
	delay = d;
}

static void
ncurses_print_stage( )
{
	move( 0, 0 );
	int nrow, ncol;
	char** board = board_BinaryLandStage( stage, &nrow, &ncol );
	int r,c;
	for( r=0; r<nrow; r++ ) {
		char buffer[ncol*2+2];
		for( c=0; c<ncol; c++ ) {
			buffer[c*2] = board[r][c];
			buffer[c*2+1] = ' ';
		}
		buffer[ncol*2] = NEWLINE;
		buffer[ncol*2+1] = 0;
		addstr( buffer );
	}
}

/// A* search with a priority queue
static bool
solve_A_search( ) 
{

}

/// bt method is basically DFS
static bool 
solve_bt( ) 
{
 


}

/// manual interaction with user input
static bool
solve_manual( )
{
	ncurses_print_stage();
	int ch;
	while( !is_solved_BinaryLandStage( stage ) ) {
		if( ( ch = getch() ) == ERR ) {
			// empty
		} else {
			switch( ch ) {
				case KEY_UP:
					move_BinaryLandStage( stage, dir_up );
					break;
				case KEY_DOWN:
					move_BinaryLandStage( stage, dir_down );
					break;
				case KEY_LEFT:
					move_BinaryLandStage( stage, dir_left );
					break;
				case KEY_RIGHT:
					move_BinaryLandStage( stage, dir_right );
					break;
				case NEWLINE:
				case CARRIAGE_RETURN:
				case KEY_ENTER:
				case KEY_ESC:
					// end manual
					return false;
			}
			ncurses_print_stage();
		}
	}

	// should print the heart on solved
	ncurses_print_stage();
	return true;
}


/// main function to start solving the stage
bool 
solve_BinaryLand( char mode )
{
	if ( stage == NULL ) {
		fprintf( stderr, "Error: Binary Land stage is not initialized for solver\n" );
		return false;
	}

	initscr();
	cbreak();
	noecho();
	keypad( stdscr, true );
	move( 0, 0 );
	refresh();

	bool is_solved = false;
	switch( mode ) {
		case 'A':
			is_solved = solve_A_search( );
			break;

		case 'B':
			is_solved = solve_bt( );
			break;
		
		case 'C':
			is_solved = solve_manual( );
			break;

		default:
			// should not reach this statement
			break;
	}

	printw( is_solved ? "Solved!\n" : "Game Over");
	getch();
	endwin();

	return is_solved;
} 
