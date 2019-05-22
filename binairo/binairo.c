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


#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "binairo_board.h"
#include "binairo_bt.h"
#include "display.h"


///
/// print_usage
///
/// prints the usage message after an error in
/// executing program occurs
///
static void print_usage(){
    fprintf( stderr, "usage: binairo [-f filename] [-d]\n" ); 
}


///
/// takes a configuration file to configure the board and
/// and performs backtracking to find solution to the puzzle
///
///
/// @return EXIT_SUCCESS if program runs without errors;
///         otherwise, EXIT_FAILURE 
///
int main( int argc, char* argv[] ){
    FILE* config_file = stdin;
    bool debug = false;
    char flag;

    while( ( flag = getopt( argc, argv, "df:" ) ) != -1 ){
        switch( flag ) {
            case 'f':
                config_file = fopen( optarg, "r" );
                if( config_file == NULL ){
                    fprintf( stderr, "Argument for (-f): No such file or directory\n" );
                    return EXIT_FAILURE;
                }
                break;
            case 'd':
                debug = true;
                break;
            case '?':
                print_usage();
                return EXIT_FAILURE;
        }

    }

    // initial board 
    BinairoBoard brd = create_BinairoBoard( config_file );

    bt_initialize( brd, debug );

    if( brd == NULL ){
        fprintf( stderr, "Error: Unable to create Binairo Board\n");
        fclose( config_file );
        return EXIT_FAILURE;
    }


    // toggle debug flag
    if( debug ){
        clear();
        if( !solve( ) ){
            set_cur_pos( 2*dim_BinairoBoard( brd )+2, 1 );
            puts( "No Solution!" );
        }
        else{
            set_cur_pos( 2*dim_BinairoBoard( brd )+2, 1 );
            puts( "Solution!" );
        }   
    }   
    else{

        // print the initial board
        puts("\nInitial Board:");
        print_BinairoBoard( brd, stdout );

        // finding a solution
        bt_initialize( brd, debug );
        if( !solve( ) ){
            printf( "\nNo Solution!\n\n" );
        }
        else{
            printf( "\nSolution:\n" );
            print_BinairoBoard( brd, stdout );  
            puts("");
        }
    }   

    destroy_BinairoBoard( brd );
    fclose( config_file );    

    return EXIT_SUCCESS;

}
