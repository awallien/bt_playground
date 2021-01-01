///
/// file: priority_queue.h
/// 
/// description:
///     Definition of a Priority Queue with the following operations:
///         - ctor: create and allocate a new Priority Queue
///         - dtor: deallocate a Priority Queue
///         - insert: add a value to Priority Queue based on key
///             - @param key: Priority Queue is ordered based on priority of key number
///             - @param value: the paired valued associated with key
///         - extract_min: pop the minimum value from the Priority Queue
///         - empty: check if a Priority Queue is empty
///         - dump: print the entire content of Priority Queue
///             - @param fp: the file pointer to dump the Priority Queue
///             - @param print_value: the function pointer to print void pointer value to fp
/// author:
///     awallien 
/// 
/// version: 1.0 - minimum implementation: insert and extract
/// 

#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_

#include <stdbool.h>
#include <stdlib.h>

typedef struct priority_queue_s* PriorityQueue;

PriorityQueue ctor_PriorityQueue();

void dtor_PriorityQueue( PriorityQueue pq );

void insert_PriorityQueue( PriorityQueue pq, size_t key, void* value );

void* extract_min_PriorityQueue( PriorityQueue pq );

bool empty_PriorityQueue( PriorityQueue pq );

void dump_PriorityQueue( PriorityQueue pq, FILE* fp, void ( *print_value )( FILE* fp, void* value ) );

#endif	///__PRIORITY_QUEUE_H_
