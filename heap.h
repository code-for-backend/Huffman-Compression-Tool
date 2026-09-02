#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_HEAP_CAPACITY 256

typedef struct heap_node
{
    uint8_t byte;
    uint32_t freq;

    struct heap_node *left;
    struct heap_node *right;

} heap_node_t;


typedef struct
{
    heap_node_t **arr;

    size_t capacity;
    size_t size;

} heap_t;


bool heap_init(heap_t *heap, size_t capacity);

bool add_node(heap_t *heap, heap_node_t *node);

heap_node_t *remove_min(heap_t *heap);


void free_heap(heap_t* heap);

#endif