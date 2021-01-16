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

#define _DEFAULT_SOURCE

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "binary_land_hash_set.h"
#include "binary_land_solve.h"
#include "binary_land_stage.h"
#include "priority_queue.h"

#define CARRIAGE_RETURN '\r'
#define NEWLINE '\n'
#define KEY_ESC 27

#define DELAY() usleep( delay )
#define POS( stage, which, buf ) pos_BinaryLandStage( stage, which, buf, buf + 1 )
#define PRINT_STAGE_DELAY() ncurses_print_stage(); DELAY()


static BinaryLandHashSet hashset = NULL;
static PriorityQueue pq = NULL;

static BinaryLandStage stage = NULL;
static char** board;
static int nrows, ncols;
static int left[2], right[2], goal[2];

static double delay = 100000;

void 
init_solve_BinaryLand( BinaryLandStage s, double d )
{
	stage = s;
	board = board_BinaryLandStage( stage, &nrows, &ncols );
	delay *= d;
}

static void
ncurses_print_stage( )
{
	move( 0, 0 );

	POS( stage, char_goal, goal );
	POS( stage, char_left_brkt, left );
	POS( stage, char_right_brkt, right );

	int r,c;
	
	char buffer[ncols*2+2];
	for( r=0; r<nrows; r++ ) {
		for( c=0; c<ncols; c++ ) {
			buffer[c*2] = board[r][c];
			buffer[c*2+1] = ' ';
		}
		
		// precedence of printing characters to board: Left, Right, Goal
		if ( r == goal[0] )
			buffer[goal[1]*2] = char_BinaryLandStage( char_goal );
		if ( r == right[0] )
			buffer[right[1]*2] = char_BinaryLandStage( char_right_brkt );
		if ( r == left[0] )
			buffer[left[1]*2] = char_BinaryLandStage( char_left_brkt );
		
		buffer[ncols*2] = NEWLINE;
		buffer[ncols*2+1] = 0;
		addstr( buffer );
	}
	refresh();
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
	PRINT_STAGE_DELAY();

	// goal
	if ( is_solved_BinaryLandStage( stage ) )
		return true;

	for( int d = dir_left; d <= dir_down; d++ ) {
		if ( can_move_BinaryLandStage( stage, d ) ) {
			move_BinaryLandStage( stage, d );
			POS( stage, char_left_brkt, left );
			POS( stage, char_right_brkt, right );
			if ( !seen_BinaryLandHashSet( hashset, left, right ) && solve_bt( ) )
				return true;
			reverse_move_BinaryLandStage( stage, d );
			PRINT_STAGE_DELAY();
		}
	}

	return false;
}

/// manual interaction with user input
static bool
solve_manual( )
{
	ncurses_print_stage();
	int ch;
	while( !is_solved_BinaryLandStage( stage ) ) {
		if( ( ch = getch() ) != ERR ) {
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
	curs_set(0);
	move( 0, 0 );
	refresh();

	bool is_solved = false;
	switch( mode ) {
		case 'A':
			is_solved = solve_A_search( );
			break;

		case 'B':
		{
			hashset = ctor_BinaryLandHashSet( nrows, ncols );
			is_solved = solve_bt( );
			dtor_BinaryLandHashSet( hashset );
			break;
		}
		case 'C':
			is_solved = solve_manual( );
			break;

		default:
			// should not reach this statement
			break;
	}

	printw( is_solved ? "Solved!\nPress Enter to Exit." : "Game Over");
	while( getch() != NEWLINE );
	endwin();

	return is_solved;
} 
