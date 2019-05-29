///
/// file: 
/// 	hash_info
///
/// author:
/// 	awallien
///
/// description:
/// 	implementation of a data structure
/// 	that contains hash information about a
/// 	binairo board's rows and column
///
/// date:
/// 	5/24/19
///


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/// data representation of the hash collection
/// that contains information of all row and column hashes
struct HashInfoStruct {
	size_t* hash_r;
	size_t* hash_c;
	int dim;
};

#include "hash_info.h"

/// create the hash info struct
HashInfo create_HashInfo( int dim ){
	HashInfo hi = malloc( sizeof( struct HashInfoStruct ) );

	assert( hi != NULL );

	hi->hash_r = calloc( dim, sizeof( size_t ) );
	hi->hash_c = calloc( dim, sizeof( size_t ) );

	assert( hi->hash_r && hi->hash_c );

	hi->dim = dim;

	return hi;	
}


/// destroy the hash info struct
void destroy_HashInfo( HashInfo hi ){
	if( hi != NULL ){
		if( hi->hash_r != NULL )
			free( hi->hash_r );
		if( hi->hash_c != NULL )
			free( hi->hash_c );
		free( hi );
	}
}


/// put a string's hash value into the hash collection
void put_HashInfo( HashInfo hi, char* s, Vector dir, int vec ){
	size_t num = strtol( s, NULL, 2 );
	switch( dir ){
		case ROW:
			hi->hash_r[vec] = num;
			break;
		case COL:
			hi->hash_c[vec] = num;
			break;
	}
}


/// get a hash value from hash collection
size_t get_HashInfo( HashInfo hi, Vector dir, int vec ){
	switch( dir ){
		case ROW:
			return hi->hash_r[vec];
		case COL:
			return hi->hash_c[vec];
	}	
	assert( !dir );
	return 0;	
}

void print_debug_HashInfo( HashInfo h ){
	int i;
	for( i=0; i<h->dim; i++ ){
		printf( "row[%d]: %lu\n", i, h->hash_r[i] ); fflush(stdout);
	}
}	
