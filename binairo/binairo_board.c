///
/// file: 
///     binairo_board.c
///
/// author: 
///     asw8675
///
/// description:
///     "class" to represent a Binairo puzzle board
///
/// date:
///     4/25/19
///

#define _DEFAULT_SOURCE
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct RowStruct {

	char* row;
	int num_of_0s;
	int num_of_1s;

} RowInfo;

/// data structure for board
struct BinairoBoardStruct {

    RowInfo** contents;     /// filled content on board, either '0' or '1'
    bool* marked;        /// is the board already marked from the given config
    size_t width;
    size_t height;

};

#include "binairo_board.h"

/// create a Binairo puzzle board from config file
BinairoBoard create_BinairoBoard( FILE* config_file ){
    
    // collect dimensions of board
    char* line = NULL;
    size_t dummy;
    getline( &line, &dummy, config_file );
    
    size_t width = strtol( line, &line, 10 );
    size_t height = strtol( line, NULL, 10 );

    if( width <= 0 ){
        fprintf( stderr, "Error: width should be greater than 0\n" );
		return NULL;
    }
	else if( height <= 0 ){
		fprintf( stderr, "Error: height should be greater than 0\n" );
		return NULL;
	}

	// create the actual binairo board
	BinairoBoard brd = malloc( sizeof( struct BinairoBoardStruct ) );
	
	assert( brd );

	brd->width = width;
	brd->height = height;

	brd->contents = calloc( width * height, sizeof( rowInfo* ) );
	brd->marked = calloc( width * height, sizeof( bool ) );

	assert( brd->contents && brd->marked );

	// make each row from config file
	for ( int i=0; i<height; i++ ){

		brd->contents[i] = malloc( sizeof( rowInfo ) );
		assert( brd->contents[i] );
		
		brd->contents[i]->row = calloc( width, sizeof( char ) );
		assert( brd->contents[i]->row );

		brd->contents[i]->num_of_0s = 0;
		brd->contents[i]->num_of_1s = 0;

		getline( &line, &dummy, config_file );

		//TODO: the rest	 



	}





    return NULL; 
    

}

/// pretty print the board
void print_BinairoBoard( BinairoBoard brd ){}


/// destroy Binairo Board
void destroy_BinairoBoard( BinairoBoard board ){}



