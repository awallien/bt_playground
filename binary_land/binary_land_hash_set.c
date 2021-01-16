///
/// file: binary_land_hash_set.c
///
/// description:
///     implementation of Binary Land hash set
///
/// author:
///     awallien
///
/// date:
///     1/9/2021
///

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct hashsetnode_s {
    int capacity;
    struct hashsetnode_s** children;
};
typedef struct hashsetnode_s* HashSetNode;

struct binarylandhashset_s {
    int nrows;
    int ncols;
    HashSetNode root;
};

#define ALLOCATE_ONE_UNIT( x ) calloc( 1, sizeof( x ) )

#define FAIL_COND_RET_NULL( cond, dtor_obj, unable )                       \
    if ( cond ) {                                                          \
        fprintf( stderr, "Unable to allocate memory for %s.\n", unable );  \
        dtor_obj;                                                          \
    }

#define LEAF 0

#include "binary_land_hash_set.h"

static void
dtor_HashSetNode( HashSetNode node )
{
    if ( node ) {
        if ( node->children ) {
            for( int i=0; i<node->capacity; i++ )
                dtor_HashSetNode( node->children[i] );
            free( node->children );
        }
        free( node );
    }

    node = NULL;
}

void
dtor_BinaryLandHashSet( BinaryLandHashSet hashset )
{
    if ( hashset ) {
        dtor_HashSetNode( hashset->root );
        free( hashset );
    }
    
    hashset = NULL;
}

static HashSetNode
ctor_HashSetNode( unsigned capacity )
{
    HashSetNode node = ALLOCATE_ONE_UNIT( struct hashsetnode_s );
    FAIL_COND_RET_NULL( !node, dtor_HashSetNode( node ), "HashSetNode" );

    if ( capacity == 0 ) {
        node->capacity = capacity;
        node->children = NULL;
    } else {
        node->capacity = capacity;
        node->children = calloc( capacity, sizeof( struct hashsetnode_s* ) );
    }

    return node;
}

BinaryLandHashSet 
ctor_BinaryLandHashSet( int nrows, int ncols )
{
    BinaryLandHashSet hs = ALLOCATE_ONE_UNIT( struct binarylandhashset_s );
    FAIL_COND_RET_NULL( !hs, dtor_BinaryLandHashSet( hs ), "BinaryLandHashSet");

    hs->nrows = nrows;
    hs->ncols = ncols;
    hs->root = ctor_HashSetNode( nrows );

    return hs;
}

bool
seen_BinaryLandHashSet( BinaryLandHashSet hashset, int left[2], int right[2] )
{
#define WITHIN_BOUND( x ) ( ( x[0] >= 0 && x[0] < hashset->nrows ) && ( x[1] >= 0 && x[1] < hashset->ncols ) )

    assert( WITHIN_BOUND( left ) && WITHIN_BOUND( right ) );
    
    HashSetNode node = hashset->root;
    bool seen = true;

    if ( !node->children[left[0]] ) {
        seen = false;
        node->children[left[0]] = ctor_HashSetNode( hashset->ncols );
    }
    node = node->children[left[0]];

    if ( !node->children[left[1]] ) {
        seen = false;
        node->children[left[1]] = ctor_HashSetNode( hashset->nrows );
    }
    node = node->children[left[1]];

    if ( !node->children[right[0]] ) {
        seen = false;
        node->children[right[0]] = ctor_HashSetNode( hashset->ncols );
    }
    node = node->children[right[0]];

    if ( !node->children[right[1]] ) {
        seen = false;
        node->children[right[1]] = ctor_HashSetNode( LEAF );
    }

    return seen;
}