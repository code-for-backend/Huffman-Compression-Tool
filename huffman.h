#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include <stdbool.h>

#include "bit_writer.h"
#include "heap.h"

typedef struct
{
    uint32_t code; 
    uint8_t len;
} huffman_code_t;

heap_node_t *build_huffman_tree(uint32_t frequency[256]);


/*for now we reject huffman code greater having len>32*/
bool generate_codes(heap_node_t *root,
                    huffman_code_t codes[256]);




bool huffman_encode(const uint8_t *data,
                    size_t size,
                    huffman_code_t codes[256],
                    bit_writer_t *writer);       


   void free_tree(heap_node_t *node); 

#endif