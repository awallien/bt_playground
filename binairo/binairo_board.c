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
    char* row;              /// contents in this row
    int num_of_0s;          /// number of 0s in row
    int num_of_1s;          /// number of 1s in row
} RowInfo;

/// representatoin of a single column on board
typedef struct ColStruct {
    char* col;				/// contents in this column
    int num_of_0s;			/// number of 0s in column
	int num_of_1s;			/// number of 1s in column
} ColInfo;

/// data structure for board
struct BinairoBoardStruct {
    RowInfo** r_contents;		/// filled content on board in rows, either '0' or '1'
	ColInfo** c_contents;		/// filed content on board in columns
    bool* marked;           	/// is the board already marked from the given config file
    int dim;                	/// dimension of the board
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

	brd->c_contents = calloc( size, sizeof( ColInfo* ) );
    brd->r_contents = calloc( size, sizeof( RowInfo* ) );
    brd->marked = calloc( size * size, sizeof( bool ) );

    assert( brd->r_contents && brd->c_contents && brd->marked );

	// make each column info
	for( size_t i=0; i<size; i++ ){
		assert( brd->c_contents[i] = malloc( sizeof( ColInfo ) ) );
		assert( brd->c_contents[i]->col = calloc( size, sizeof( char ) ) );	
		brd->c_contents[i]->num_of_0s = 0;
		brd->c_contents[i]->num_of_1s = 0;	
	}

    // make each row from config file
    for ( size_t i=0; i<size; i++ ){

        brd->r_contents[i] = malloc( sizeof( RowInfo ) );
        assert( brd->r_contents[i] );
        
        brd->r_contents[i]->row = calloc( size, sizeof( char ) );
        assert( brd->r_contents[i]->row );

        brd->r_contents[i]->num_of_0s = 0;
        brd->r_contents[i]->num_of_1s = 0;

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
                    brd->r_contents[i]->row[j] = '.';
					brd->c_contents[j]->col[i] = '.';
                    brd->marked[i*size+j] = false;
                    break;

                case '0':
                    brd->r_contents[i]->row[j] = '0';
                    brd->r_contents[i]->num_of_0s++;
					brd->c_contents[j]->col[i] = '0';
					brd->c_contents[j]->num_of_0s++;
                    brd->marked[i*size+j] = true;
                    break;

                case '1':
                    brd->r_contents[i]->row[j] = '1';
                    brd->r_contents[i]->num_of_1s++;
                    brd->c_contents[j]->col[i] = '1';
                    brd->c_contents[j]->num_of_1s++;
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
/// @param dim      the dimension of the board
/// @param stream   the place to print the border
///
static void print_border( size_t dim, FILE* stream ){
    fputc( '+', stream );
    for( size_t j=0; j<dim; j++ )
        fputs( "-+", stream );

    fputc( '\n', stream );  
}


/// pretty print the board
void print_BinairoBoard( BinairoBoard brd, FILE* stream ){
    int i,j;

    // print top border
    print_border( brd->dim, stream );

    // print contents
    for( i=0; i<brd->dim; i++ ){
        fputc( '|', stream );
        for( j=0; j<brd->dim; j++ ) 
            fprintf( stream, "%c|", brd->r_contents[i]->row[j] );
        fputc( '\n', stream );
        
        // print inner (and bottom) border
        print_border( brd->dim, stream );   
    }
}


/// destroy Binairo Board
///
/// free:
///     brd->marked
///     brd->contents
///         contents->row
void destroy_BinairoBoard( BinairoBoard brd ){
    if( brd->marked != NULL )
        free( brd->marked );

    if( brd->r_contents != NULL ){
        for( int i=0; i<brd->dim; i++ ){
            if( brd->r_contents[i] != NULL ){
                if( brd->r_contents[i]->row != NULL )
                    free( brd->r_contents[i]->row );
                free( brd->r_contents[i] );
            }
        }
        free( brd->r_contents );
    }

	if( brd->c_contents != NULL ){
		for( int i=0; i<brd->dim; i++ ){
			if( brd->c_contents[i] != NULL ){
				if( brd->c_contents[i]->col != NULL )
					free( brd->c_contents[i]->col );
				free( brd->c_contents[i] );
			}
		}
		free( brd->c_contents );
	}

    free( brd );
}


/// the dimension of the board
int dim_BinairoBoard( BinairoBoard b ){ 
    assert( b );
    return b->dim; 
}


/// is the cell initially marked on the board?
bool is_marked_BinairoBoard( BinairoBoard b, int cell ){ 
    assert( b );
    return b->marked[cell];
}


#define COL( b, cell ) 		b->c_contents[cell%b->dim]
#define ROW( b, cell )      b->r_contents[cell/b->dim]  
#define SPOT( b, cell )     b->r_contents[cell/b->dim]->row[cell%b->dim]
#define CPOT( b, cell )		b->c_contents[cell%b->dim]->col[cell/b->dim]

/// put digit on board
void put_BinairoBoard( BinairoBoard b, int cell, Digit digit ){
    assert( b );

    // check spot for digit for update
    switch( SPOT( b, cell ) ){
        case '0':
            ROW( b, cell )->num_of_0s--;
			COL( b, cell )->num_of_0s--;
            break;

        case '1':
            ROW( b, cell )->num_of_1s--;
			COL( b, cell )->num_of_1s--;
            break;
    }

    // update cell and counter
    switch( digit ){
        case ZERO:
			CPOT( b, cell ) = '0';
            SPOT( b, cell ) = '0';
            ROW( b, cell )->num_of_0s++;
			COL( b, cell )->num_of_0s++;
            break;

        case ONE:
			CPOT( b, cell ) = '1';
            SPOT( b, cell ) = '1';
            ROW( b, cell )->num_of_1s++;
			COL( b, cell )->num_of_1s++;
            break;

        case BLANK:
            SPOT( b, cell ) = '.';

    }
}


/// get digit from a cell on board
Digit get_BinairoBoard( BinairoBoard b, int cell ){
    assert( b );
    char d = SPOT( b, cell );
    switch( d ){
        case '0':
            return ZERO;
        case '1':
            return ONE;
        default:
            return BLANK;
    }
}


/// get number of digit in a row
int numberof_BinairoBoard( BinairoBoard b, int row_number, Digit d  ){
    assert( b );
    switch( d ){
        case ONE:
            return b->r_contents[row_number]->num_of_1s;
        case ZERO:
            return b->r_contents[row_number]->num_of_0s;
        case BLANK:
            return b->dim - b->r_contents[row_number]->num_of_0s -
                        b->r_contents[row_number]->num_of_1s;

    }
    return 0;
}
