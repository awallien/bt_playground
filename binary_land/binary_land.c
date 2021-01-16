///
/// file:
/// 	binary_land.c
///
/// author:
/// 	awallien
/// 
/// description:
/// 	solver for a simpler (i.e. no enemies, webs, and items) version of
/// 	<a href="www.strategywiki.org/wiki/Binary_Land>Binary Land</a>
///
/// date:
/// 	9/28/19
///


#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_land_solve.h"
#include "binary_land_stage.h"


/// print usage to stderr
#define PRINT_USAGE \
	fprintf( stderr, "\nUsage: binary_land [-f filename] [-m A (A* search) | B (bt) | C (manual)] [-a delay]\n\n" )

#define CLOSE_FILE( f ) \
	if ( f ) fclose( f );

/// main function that
/// 	- reads input
/// 	- accounts for allocating and freeing BinaryLand objects
/// 	- outputs result of whether the stage given to this program is solvable 
int main(int argc, char* argv[])
{
	FILE* config_file = stdin;
	char mode = 'B';	
	double delay = 1;

	char flag;
	while( ( flag = getopt( argc, argv, "a:m:f:" ) ) != -1 ){
		switch( flag ) {
			case 'a':
			{
				if ( optarg ) {
					delay = strtod( optarg, &optarg );
					if ( *optarg ) {
						CLOSE_FILE( config_file );
						fprintf( stderr, "Error: invalid delay value\n" );
						return EXIT_FAILURE;
					}	
					delay = delay > 0 ? delay : 1;
				}
				break;	
			}
			case 'm':
			{
				int optarg_len = strlen(optarg);
				if ( optarg_len != 1 || ( *optarg != 'A' && *optarg != 'B' && *optarg != 'C' ) ) {
					fprintf( stderr, "Error: Invalid value for mode: %s\n", optarg );
					return EXIT_FAILURE;
				}
				mode = *optarg;
				break;
			}
			case 'f':
			{
				config_file = fopen( optarg, "r" );
				if( config_file == NULL ) { 
					fprintf( stderr, "File (%s) does not exist.\n", optarg );
					return EXIT_FAILURE;
				}
				break;
			}
			case '?':
			{
				PRINT_USAGE;
				return EXIT_FAILURE;
			}
		}
	}

	if ( config_file == stdin ) 
		fprintf( stdin, "Enter configuration below: \n" );
	
	BinaryLandStage stage = ctor_BinaryLandStage( config_file );
	CLOSE_FILE( config_file );

	if( stage == NULL )
		return EXIT_FAILURE;

	init_solve_BinaryLand( stage, delay );
	solve_BinaryLand( mode );

	dtor_BinaryLandStage(stage);

	return EXIT_SUCCESS;
}
