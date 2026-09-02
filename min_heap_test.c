#include <stdio.h>
#include <stdlib.h>

#include "heap.h"


static heap_node_t *create_node(uint8_t byte, uint32_t freq)
{
    heap_node_t *node = malloc(sizeof(heap_node_t));

    if (node == NULL)
    {
        return NULL;
    }

    node->byte = byte;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;

    return node;
}


int main(void)
{
    heap_t heap;

    heap_node_t *node;

    if (!heap_init(&heap, MAX_HEAP_CAPACITY))
    {
        printf("Heap initialization failed\n");
        return 1;
    }

    /* Insert test nodes */

    add_node(&heap, create_node('A', 45));
    add_node(&heap, create_node('B', 13));
    add_node(&heap, create_node('C', 12));
    add_node(&heap, create_node('D', 16));
    add_node(&heap, create_node('E', 9));
    add_node(&heap, create_node('F', 5));

    printf("Heap size: %zu\n\n", heap.size);

    printf("Removing nodes:\n");

    while (heap.size > 0)
    {
        node = remove_min(&heap);

        printf("Byte: '%c'  Frequency: %u\n",
               node->byte,
               node->freq);

        free(node);
    }

    free(heap.arr);

    return 0;
}