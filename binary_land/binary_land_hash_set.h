///
/// file: binary_land_hash_set.h
///
/// description:
///     header file containing the definitions and functions
///     of a Binary Land hash set
///
/// author:
///     awallien
///
/// date:
///     1/9/2021
///

#ifndef __BINARY_LAND_HASH_SET_H_
#define __BINARY_LAND_HASH_SET_H_

#include <stdbool.h>

/// object representation of Binary Land Hash
typedef struct binarylandhashset_s* BinaryLandHashSet;

///
/// ctor_BinaryLandHashSet
///
/// allocate new Binary Land hashset
///
/// @param nrows the number of rows
///
/// @param ncols the number of columns
///
/// @return the new object
///
BinaryLandHashSet ctor_BinaryLandHashSet( int nrows, int cols );

///
/// dtor_BinaryLandHashSet
///
/// deallocate Binary Land hashset object
///
/// @param hashset the object
///
void dtor_BinaryLandHashSet( BinaryLandHashSet hashset );

///
/// check_BinaryLandHashSet
///
/// check if the coordinates of both the left and right brackets are already in
/// the hash set; if not seen, then this set of coordinates would be marked 'seen' by
/// this function
///
/// @param hashset the Binary Land hash set
///
/// @param left left bracket's x [0] and y [1] position
/// 
/// @param right right bracket's x [0] and y [1] position
///
/// @return true if coordinate points are in hash set; otherwise, false
///
bool seen_BinaryLandHashSet( BinaryLandHashSet hashset, int left[2], int right[2] );

#endif /// __BINARY_LAND_HASH_SET_H_