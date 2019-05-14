///
/// file: 
///     binairo.c
///
/// author: 
///     asw8675
///
/// description:
///     solver for Binairo (Takuzu)
///
/// date:
///     4/25/19
///

#include <stdio.h>
#include <stdlib.h>

#include "binairo_board.h"

///
/// takes a configuration file to configure the board and
/// and performs backtracking to find solution to the puzzle
///
/// @param argc - number of arguments: 1
/// @param argv - [1] the configuration file
///
/// @return EXIT_SUCCESS if program runs without errors;
///         otherwise, EXIT_FAILURE 
///
int main( int argc, char* argv[] ){

	// arg and file checking
    if ( argc != 2 ){
        fprintf( stderr, "Usage: binario config-file\n" );
        return EXIT_FAILURE;
    }

    FILE* config_file = fopen( argv[1], "r" );
    if( config_file == NULL ){
        fprintf( stderr, "%s: No such file or directory\n", argv[1] );
        return EXIT_FAILURE; 
    }

   	// initial board 
    BinairoBoard brd = create_BinairoBoard( config_file );
 
	if( brd == NULL ){
        fprintf( stderr, "Error: Unable to create Binairo Board\n");
		fclose( config_file );
		return EXIT_FAILURE;
	}

	print_BinairoBoard( brd, stdout );
	destroy_BinairoBoard( brd );

    fclose( config_file );    

    return EXIT_SUCCESS;

}
