///
/// file:
///     binairo_board.h
///
/// description:
///     header file that contains the data structure and implementations
///     for a Binairo puzzle board
/// 
/// author:
///     asw8675
///
/// date:
///     4/26/19
///

#ifndef _BINAIRO_BOARD_H
#define _BINAIRO_BOARD_H

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
/// @pre the config_file is a valid FILE*
///
BinairoBoard create_BinairoBoard( FILE* config_file );


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
