///
/// file:
///		hash_info.h
///
///	author:
/// 	awallien
///
/// description:
/// 	data structure representation of a hash collection
/// 	for checking duplicate strings
///
/// date:
/// 	5/27/19
/// 		


#ifndef _HASH_INFO_H
#define _HASH_INFO_H

#include <stdlib.h>

/// Direction to insert a hash into the hash collection
typedef enum Vector_e { ROW, COL } Vector;

/// data structure declaration for the hash collection
typedef struct HashInfoStruct* HashInfo;


///
/// create_HashInfo
///
/// allocate memory to create the hash collection data
/// structure declare above
///
/// @param dim - the dimension of the binairo board
///
/// @return - the hash info collection
///
HashInfo create_HashInfo( int dim );


///
/// destroy_HashInfo
///
/// free hash collection data structure that memory allocated
///
/// @param hashinfo - the hash collection structure to be freed
///
void destroy_HashInfo( HashInfo hashinfo );


///
/// put_HashInfo
///
/// compute the given string's hash and store it in the hash collection
/// with the given direction and cell (status) number
///
/// @param hashinfo - the hash collection data structure
/// @param string - the string to compute hash and store in hash collection
/// @param direction - a vector direction, either row or column
/// @param index - the row or column to get the hash value from
///
void put_HashInfo( HashInfo hashinfo, char* string, Vector direction, int index ); 


///
/// get_HashInfo
///
/// get the hash value from a hash collection data structure
/// 
/// @param hashinfo - the hash collection data structure
/// @param direction - the vector direction to get hash value
/// @param index - the row or column to get the hash value from
///
/// @return - the hash value
///
size_t get_HashInfo( HashInfo hashinfo, Vector direction, int index );


#endif //_HASH_INFO_H
