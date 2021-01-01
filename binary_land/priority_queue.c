///
/// file: priority_queue.c
///
/// description: 
///     Implementation of Priority Queue structure
///
/// author: 
///     awallien
///
/// version: 1.0
/// 

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

/// definition of a single node in Priority Queue
struct priority_queue_node_s 
{
    size_t degree;
    size_t key;
    void* value;

    struct priority_queue_node_s* left;
    struct priority_queue_node_s* right;
    struct priority_queue_node_s* parent;
    struct priority_queue_node_s* child;
};
typedef struct priority_queue_node_s* PQNode;

/// internal definition of Priority Queue structure
struct priority_queue_s 
{
    /** number of nodes in Priority Queue */
    size_t size;

    /** number of nodes in the root list of Priority Queue */
    size_t root_list_size;

    /** the minimum node in the Priority Queue */
    PQNode min;
};

#define ALLOCATE_ONE_UNIT(x) (x*) calloc( 1, sizeof(x) )
#define ASSERT_PQ assert(pq)

#include "priority_queue.h"

PriorityQueue 
ctor_PriorityQueue()
{
    PriorityQueue pq = ALLOCATE_ONE_UNIT( struct priority_queue_s );
    if ( !pq ) {
        fprintf( stderr, "Unable to allocate memory for PriorityQueue\n" );
        return NULL;
    }

    pq->size = 0;
    pq->min = NULL;

    return pq;
}

static PQNode
ctor_PQNode( size_t key, void* value )
{
    PQNode node = ALLOCATE_ONE_UNIT( struct priority_queue_node_s );
    if ( !node ) {
        fprintf( stderr, "Unable to allocate memory for PriorityQueue Node\n");
        return NULL;
    }

    node->degree = 0;
    node->key = key;
    node->value = value;

    node->left = node;
    node->right = node;
    node->child = NULL;
    node->parent = NULL;

    return node;
}

static void
dtor_PQNode( PQNode node )
{
    if ( node ) {
        // traverse through current node neighbors and free their child and node
        PQNode current = node;
        while( current->right != node ) {
            PQNode next = current->right;
            dtor_PQNode( current->child );
            free( current );
            current = next;
        } 
        free( current );
    }

    node = NULL;
}

void 
dtor_PriorityQueue( PriorityQueue pq )
{
    if ( pq ) {
        dtor_PQNode( pq->min );
        free( pq );
    }

    pq = NULL;
}

/** HELPER FNS */

///
/// merge_node_to_list
///
/// merge a node into the first index of the given list
///
/// @param to_ptr pointer to the head of PQNode list
/// @param node the node to merge into the list
///
static void
merge_node_to_list( PQNode* to_ptr, PQNode node )
{
    PQNode to = *to_ptr;
    if ( !to ) {
        *to_ptr = node;
    } else {
        node->right = to->right;
        node->left = to;
        to->right->left = node;
        to->right = node; 
    }
}

///
/// merge_node_to_child_list
///
/// make a node a child of another node
///
/// @param parent parent node
/// @param node the child node
///
static void
merge_node_to_child_list( PQNode parent, PQNode node )
{
    if ( !parent->child ) {
        parent->child = node;
    } else {
        node->right = parent->child->right;
        node->left = parent->child;
        parent->child->right->left = node;
        parent->child->right = node;
    }
}

///
/// remove_node_from_list
///
/// remove node from its contained list
///
/// @param node the given node
///
static void
remove_node_from_list( PQNode node )
{
    PQNode
        left = node->left,
        right = node->right;
    
    left->right = right;
    right->left = left;
}

///
/// heap_link
///
/// append node to child list of another node
///
/// @param y the child node
/// @param x the parent node
///
static void
heap_link( PQNode y, PQNode x )
{
    remove_node_from_list( y );
    y->left = y;
    y->right = y;
    merge_node_to_child_list( x, y );
    ++x->degree;
    y->parent = x;
}

///
/// consolidate
///
/// after extracting the minimum node, balance the Priority Queue
/// based on the ranks of the nodes
///
/// @param pq PriorityQueue
///
static void
consolidate( PriorityQueue pq )
{
    ASSERT_PQ;
    
    size_t ranks_len = (size_t)( log( pq->size ) / log( 2 ) + 1.0 );
    PQNode ranks[ranks_len];
    size_t i;

    for( i=0; i<ranks_len; i++ ) {
        ranks[i] = NULL;
    }

    PQNode w;
    for( i=0, w=pq->min; i<pq->root_list_size; i++, w = w->right) {
        size_t d = w->degree;

        while( ranks[d] && ranks[d] != w ) {
            PQNode y = ranks[d];

            if( y->key < w->key ) {
                PQNode temp = w;
                w = y;
                y = temp;
            }

            heap_link( y, w );
            ranks[d] = NULL;
            ++d;
        }

        ranks[d] = w;
        if ( w->key <= pq->min->key )
            pq->min = w;
        
    }   

    pq->root_list_size = ranks_len; 
}

/** HELPER FNS END */

void 
insert_PriorityQueue( PriorityQueue pq, size_t key, void* value )
{
    ASSERT_PQ;

    PQNode new_node = ctor_PQNode( key, value );
    
    merge_node_to_list( &(pq->min), new_node );
    
    if ( !pq->min || key <= pq->min->key ) { 
        pq->min = new_node;
    }

    ++pq->size;
    ++pq->root_list_size;
}

void* 
extract_min_PriorityQueue( PriorityQueue pq )
{
    ASSERT_PQ;
    PQNode m = pq->min;
    void* res = NULL;
    
    if ( m ) {
        res = m->value;
        PQNode min_child = m->child;

        if ( min_child ) {
            PQNode stop = min_child->left;

            while ( min_child != stop ) {
                min_child->parent = NULL;
                PQNode temp = min_child->right;
                merge_node_to_list( &(pq->min), min_child );
                min_child = temp;
            }
            stop->parent = NULL;
            merge_node_to_list( &(pq->min), stop );
        }

        remove_node_from_list( m );
        --pq->size;

        // check if only element in root list
        if ( m == m->right ) {
            pq->min = NULL;
        } else {
            pq->min = m->right;
            consolidate( pq );
        }

        free( m );
        m = NULL;
    }

    return res;
}

bool 
empty_PriorityQueue( PriorityQueue pq )
{
    return pq->size == 0;
}

static void
dump_rec_PQNode( PQNode node, FILE* fp, void ( *print_value )( FILE*, void* ), size_t depth )
{
    if ( node ) {
        PQNode stop = node;
        do {
            for( size_t i=0; i<depth; i++ )
                fprintf( fp, "    " );
            fprintf( fp, "<key: %lu, value: ", node->key );
            print_value( fp, node->value );
            fprintf( fp, ">\n" );
            dump_rec_PQNode( node->child, fp, print_value, depth+1 );
        } while( stop != ( node = node->left ) );
    }
}

void 
dump_PriorityQueue( PriorityQueue pq, FILE* fp, void ( *print_value )( FILE*, void* ) )
{
    fprintf( fp, "<size: %lu>\n", pq->size );
    dump_rec_PQNode( pq->min, fp, print_value, 0 );
    fprintf( fp, "\n" );
}
