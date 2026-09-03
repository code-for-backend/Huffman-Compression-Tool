#include <stdio.h>

#include "bit_writer.h"

int main(void)
{
    bit_writer_t writer;

    bit_writer_init(&writer);

    /*
     * A → 0
     * B → 10
     * C → 11
     *
     * ABCAB
     *
     * 0 | 10 | 11 | 0 | 10
     *
     * 01011010
     */

    bit_writer_write_code(&writer, 0, 1);  // A
    bit_writer_write_code(&writer, 2, 2);  // B = 10
    bit_writer_write_code(&writer, 3, 2);  // C = 11
    bit_writer_write_code(&writer, 0, 1);  // A
    bit_writer_write_code(&writer, 2, 2);  // B = 10

    bit_writer_flush(&writer);

    return 0;
}