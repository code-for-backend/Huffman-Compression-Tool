#ifndef BIT_WRITER_H
#define BIT_WRITER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    uint8_t buffer;
    uint8_t bit_count;
    FILE* fp;
} bit_writer_t;

void bit_writer_init(bit_writer_t *writer,FILE* fp);

bool bit_writer_write_bit(bit_writer_t *writer, uint8_t bit);

bool bit_writer_write_code(bit_writer_t *writer,
                           uint32_t code,
                           uint8_t len);

bool bit_writer_flush(bit_writer_t *writer);

#endif