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


#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/// representation of a single row on a board
typedef struct RowStruct {

	char* row;				/// contents in this row
	size_t num_of_0s;		/// number of 0s in row
	size_t num_of_1s;		/// number of 1s in row

} RowInfo;


/// data structure for board
struct BinairoBoardStruct {

    RowInfo** contents;     /// filled content on board, either '0' or '1'
    bool* marked;        	/// is the board already marked from the given config file
    size_t dim;				/// dimension of the board

};


#include "binairo_board.h"
#include "get_line.h"


/// create a Binairo puzzle board from config file
BinairoBoard create_BinairoBoard( FILE* config_file ){
    
    // collect dimensions of board
    char* line = NULL;
    size_t dummy;
    get_line( &line, &dummy, config_file );
    
	if ( line == NULL ){
		fprintf( stderr, "Error: unable to read size of board.\n");
		return NULL;
	}

    size_t size = strtol( line, NULL, 10 );

	free( line );

	if( size <= 1 ){
        fprintf( stderr, "Error: dimension of board should be greater than 1.\n" );
		return NULL;
    }
	
	// create the actual binairo board
	BinairoBoard brd = malloc( sizeof( struct BinairoBoardStruct ) );
	
	assert( brd );

	brd->dim = size;

	brd->contents = calloc( size * size, sizeof( RowInfo* ) );
	brd->marked = calloc( size * size, sizeof( bool ) );

	assert( brd->contents && brd->marked );

	// make each row from config file
	for ( size_t i=0; i<size; i++ ){

		brd->contents[i] = malloc( sizeof( RowInfo ) );
		assert( brd->contents[i] );
		
		brd->contents[i]->row = calloc( size, sizeof( char ) );
		assert( brd->contents[i]->row );

		brd->contents[i]->num_of_0s = 0;
		brd->contents[i]->num_of_1s = 0;

		char* line = NULL;
		size_t dummy;
		get_line( &line, &dummy, config_file );

		// simple invalid checking
		// linux - does not include /r (+1 for \n)
		// windows - does include /r (+2)
		if( strlen( line ) < size + 1 ){
			fprintf( stderr, "Error: line %zu of configuration file is invalid.\n", i+1 );
			free( line );
			destroy_BinairoBoard( brd );
			return NULL;			
		}

		// populate contents and marked
		for ( size_t j=0; j<size; j++ ){

			switch( line[j] ){

				case '.':
					brd->contents[i]->row[j] = '.';
					brd->marked[i*size+j] = false;
					break;

				case '0':
					brd->contents[i]->row[j] = '0';
					brd->contents[i]->num_of_0s++;
					brd->marked[i*size+j] = true;
					break;

				case '1':
					brd->contents[i]->row[j] = '1';
					brd->contents[i]->num_of_1s++;
					brd->marked[i*size+j] = true;
					break;

				// unknown character found
				default:
					fprintf( stderr, "Error: invalid character found on line %zu.\n", i+1 );
					destroy_BinairoBoard( brd );
					free( line );
					return NULL;

			}

		}

		free( line );

	}

    return brd;     

}


///
/// print_border
///
/// helper function to print the border of the binairo board
///
/// @param dim		the dimension of the board
/// @param stream	the place to print the border
///
static void print_border( size_t dim, FILE* stream ){

	fputc( '+', stream );
	for( size_t j=0; j<dim; j++ )
		fputs( "-+", stream );

	fputc( '\n', stream );	
}


/// pretty print the board
void print_BinairoBoard( BinairoBoard brd, FILE* stream ){

	size_t i,j;

	// print top border
	print_border( brd->dim, stream );

	// print contents
	for( i=0; i<brd->dim; i++ ){
		fputc( '|', stream );
		for( j=0; j<brd->dim; j++ ) 
			fprintf( stream, "%c|", brd->contents[i]->row[j] );
		fputc( '\n', stream );
		
		// print inner (and bottom) border
		print_border( brd->dim, stream );	
	}

}


/// destroy Binairo Board
///
/// free:
///		brd->marked
///		brd->contents
///			contents->row
void destroy_BinairoBoard( BinairoBoard brd ){

	if( brd->marked != NULL )
		free( brd->marked );

	if( brd->contents != NULL ){

		for( size_t i=0; i<brd->dim; i++ ){

			if( brd->contents[i] != NULL ){

				if( brd->contents[i]->row != NULL )
					free( brd->contents[i]->row );

				free( brd->contents[i] );

			}

		}

		free( brd->contents );
	}

	free( brd );

}


/// the dimension of the board
size_t dim_BinairoBoard( BinairoBoard brd ){ 
		return brd->dim; 
}


/// is the cell initially marked on the board?
bool is_marked_BinairoBoard( BinairoBoard b, size_t cell ){ 
		return b->marked[cell];
}



