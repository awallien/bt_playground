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


#include <stdbool.h>    // bool
#include <stdlib.h> // FILE


/// enumeration representing the valid digits on board
/// ZERO    - '0'
/// ONE     - '1'
/// BLANK   - '.'
enum Digit_e { ZERO, ONE, BLANK };
typedef enum Digit_e Digit;

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
int dim_BinairoBoard( BinairoBoard brd );


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
bool is_marked_BinairoBoard( BinairoBoard board, int cell );


///
/// put_BinairoBoard
///
/// put a digit on a binairo board
///
/// @param board - the binairo board
/// @param cell - the cell to put the digit on
/// @param digit - the specified digit
///
void put_BinairoBoard( BinairoBoard board, int cell, Digit digit );


///
/// get_BinairoBoard
///
/// get a digit from a cell on the board
///
/// @param board - the binairo board
/// @param cell  - the spot on the board
///
/// @return the digit at that cell on the board
///
Digit get_BinairoBoard( BinairoBoard board, int cell );


///
/// numberof_row_BinairoBoard
///
/// get the number of a specified digit in a row on the board
///
/// @param board - the binairo board
/// @param row   - the row number
/// @param digit - the specified digit
///
/// @return the number of specified digit in a row
///
int numberof_row_BinairoBoard( BinairoBoard board, int row, Digit digit );


///
/// numberof_col_BinairoBoard
///
/// get the number of specified digit in a column on the board
///
/// @param board - the binairo board
/// @param column - the column number
/// @param digit - the specified digit
///
/// @return the number of the digit in a column
///
int numberof_column_BinairoBoard( BinairoBoard board, int column, Digit digit );


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
