///
/// file:
///		binary_land_stage.h
///	
/// description: 
///		header file containing definitions and methods of
///		BinaryLandStage
///
/// author:
/// 	awallien
///
/// date:
/// 	12/29/2020
///

#ifndef __BINARY_LAND_STAGE_H_
#define __BINARY_LAND_STAGE_H_

#include <stdbool.h>
#include <stdio.h>

#define u_int unsigned int

/// object representation of a binary land stage
typedef struct binarylandstage_s* BinaryLandStage;

/// enum representation of the 4 cardinal directions
typedef enum { dir_left, dir_right, dir_up, dir_down } Direction;

/// enum representation of the different characters
typedef enum { char_left_brkt, char_right_brkt, char_goal } Character;

///
/// ctor_BinaryLandStage
///
/// allocates memory for a new BinaryLandStage
///
/// @param pfile the file containing input info about the board
///
/// @return instance of BinaryLandStage
///
BinaryLandStage ctor_BinaryLandStage( FILE* pfile );

/// 
/// ctor_moves_BinaryLandStage
///
/// allocates memory for number of moves (or hops) to get the brackets to goal
///
/// @param stage the Binary Land stage
///
/// @return instance of Binary Land stage's number of moves board
/// 
u_int ** ctor_moves_BinaryLandStage( BinaryLandStage stage );

///
/// dtor_BinaryLandStage
///
/// deallocates memory for a BinaryLandStage instance
///
/// @param stage the Binary Land stage
///
void dtor_BinaryLandStage( BinaryLandStage stage );

///
/// debug_BinaryLandStage
///
/// debug full board to an output file
///
/// @param stage the Binary Land stage
///
/// @param out the file to print the output
///
//void debug_BinaryLandStage( BinaryLandStage stage, FILE* out );

///
/// move_BinaryLandStage
///
/// moves the left bracket on screen with the given direction;
/// right bracket moves in polar opposite in west/east direction
///
/// @param stage the Binary Land stage
///
/// @param dir direction to move the left bracket
///
/// @return EXIT_SUCCESS if successful move; otherwise, EXIT_FAILURE
///
bool move_BinaryLandStage( BinaryLandStage stage, Direction dir );

///
/// can_move_BinaryLandStage
///
/// given a direction, do one or both of the brackets change position?
///
/// @param stage Binary Land stage
/// 
/// @param dir direction to move left bracket
/// 
/// @return true if one of the brackets change position; otherwise, false
///
bool can_move_BinaryLandStage( BinaryLandStage stage, Direction dir );

///
/// reverse_move_BinaryLandStage
///
/// given a direction, move the brackets in the opposite, given direction
///
/// @param stage Binary Land stage
///
/// @param dir direction to move left bracket
///
/// @return true if move can be made; otherwise, false
///
bool reverse_move_BinaryLandStage( BinaryLandStage stage, Direction dir );

///
/// board_BinaryLandStage
///
/// returns the entire board of the Binary Land stage
///
/// @param stage the BinaryLandStage
///
/// @param nrows_ptr a pointer to an integer to store the number of rows
///
/// @param ncols_ptr a pointer to an integer to store the number of columns
///
/// @return the 2d array stage
///
char** board_BinaryLandStage( BinaryLandStage stage, int* nrows_ptr, int* ncols_ptr );

///
/// char_BinaryLandStage
///
/// retrieves the character representation of a Character enum value
///
/// @param c the Character
///
/// @return the char representation
///
char char_BinaryLandStage( Character c );

///
/// pos_BinaryLandStage
///
/// get the position of certain character
///
/// @param stage the Binary Land stage
/// 
/// @param chr the character
///
/// @param row_pos pointer to store the row position of character
///
/// @param col_pos pointer to store the column position of character
////
void pos_BinaryLandStage( BinaryLandStage stage, Character chr, int* row_pos, int* col_pos );

///
/// is_solved_BinaryLandStage
///
/// are the brackets able to encapsulate the GOAL?
///
/// @param stage the Binary Land Stage
///
/// @return true if the brackets are at the goal; otherwise false
///
bool is_solved_BinaryLandStage( BinaryLandStage stage );

#endif
