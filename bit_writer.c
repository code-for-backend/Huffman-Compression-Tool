#include "bit_writer.h"
#include <stdio.h>

void bit_writer_init(bit_writer_t *writer,FILE* fp)
{
    writer->buffer = 0;
    writer->bit_count = 0;
    writer->fp=fp;
}

bool bit_writer_write_bit(bit_writer_t *writer, uint8_t bit)
{
    if (bit > 1)
        return false;

    if (bit)
        writer->buffer |= (uint8_t)(1 << (7 - writer->bit_count));

    writer->bit_count++;

    if (writer->bit_count == 8)
    {
        fputc(writer->buffer,writer->fp);/*flush the data to file*/

        writer->buffer = 0;
        writer->bit_count = 0;
    }

    return true;
}

bool bit_writer_write_code(bit_writer_t *writer,
                           uint32_t code,
                           uint8_t len)
{
    if (len > 32)
        return false;

    for (int i = len - 1; i >= 0; i--)
    {
        uint8_t bit = (code >> i) & 1;

        if (!bit_writer_write_bit(writer, bit))
            return false;
    }

    return true;
}


bool bit_writer_flush(bit_writer_t *writer)
{
    if (writer->bit_count == 0)
        return true;

    fputc(writer->buffer,writer->fp);

    writer->buffer = 0;
    writer->bit_count = 0;

    return true;
}