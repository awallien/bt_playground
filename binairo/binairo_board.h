///
/// file:
///     binairo_board.h
///
/// description:
///     header file that contains the data structure and implementations
///     for a Binairo puzzle board; follows ADT Management
/// 
/// author:
///     asw8675
///
/// date:
///     4/26/19
///


#ifndef _BINAIRO_BOARD_H
#define _BINAIRO_BOARD_H


#include <stdbool.h>
#include <stdlib.h>


/// data structure type for a Binairo Board
typedef struct BinairoBoardStruct* BinairoBoard;


///
/// create_BinairoBoard
///
/// create the puzzle board with the given configurations from
/// the file
///
/// @param config_file - configuration file
/// 
/// @return - the puzzle board
///
/// @pre the config_file is a valid FILE*, not NULL
///
BinairoBoard create_BinairoBoard( FILE* config_file );


///
/// dim_BinairoBoard
///
/// the dimension of the board
///
/// @param board - the binairo board
/// 
/// @return - the dimension of the board
///
size_t dim_BinairoBoard( BinairoBoard brd );


///
/// is_marked_BinairoBoard
///
/// determine if a cell on the board is already marked
/// from the initial board
///
/// @param board - the binairo board
/// @param cell - the cell spot to check
///
/// @return - true if the cell on board is initially marked; otherwise, false
///
bool is_marked_BinairoBoard( BinairoBoard board, size_t cell );


///
/// put_BinairoBoard
///
/// put a digit on a binairo board
///
/// @param board - the binairo board
/// @param cell - the cell to put the digit on
/// @param digit - the digit
///
/// @pre - the cell is a valid number within the
///		boundary of the board
/// @pre - the digit is valid, either a '0' or '1'
///
void put_BinairoBoard( BinairoBoard board, size_t cell, char digit );


///
/// erase_BinairoBoard
///
/// erase a digit on a binairo board
///
/// @param board - the binairo board
/// @param cell - the cell to erase a digit
///
/// @pre - the cell is a valid number within the
///		boundary of the board
///
void erase_BinairoBoard( BinairoBoard board, size_t cell );


///
/// print_BinairoBoard
///
/// pretty prints the binairo board to desired output stream
///
/// @param board - the board to print out
///
void print_BinairoBoard( BinairoBoard board, FILE* stream );


///
/// destroy_BinairoBoard
///
/// destroys the specified heap, deallocating any
/// allocated memory
///
/// @param board - the board to free
/// 
void destroy_BinairoBoard( BinairoBoard board );


#endif  //_BINAIRO_BOARD_H
