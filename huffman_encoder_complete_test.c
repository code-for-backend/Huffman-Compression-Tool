
#include <stdio.h>
#include <stdint.h>

#include "huffman.h"
#include "bit_writer.h"

/*This is the finished and complete final huffman compression project*/

int main(void)
{
    FILE *input = fopen("input.txt", "rb");

    if (input == NULL)
    {
        perror("input.txt");
        return 1;
    }

    uint32_t frequency[256] = {0};

    /*
     * Build frequency table.
     */
    int ch;

    while ((ch = fgetc(input)) != EOF)
        frequency[(uint8_t)ch]++;

    /*
     * Input is now at EOF.
     * Rewind it so that we can encode it.
     */
    rewind(input);

    /*
     * Build Huffman tree.
     */
    heap_node_t *root = build_huffman_tree(frequency);

    if (root == NULL)
    {
        printf("Failed to build Huffman tree\n");
        fclose(input);
        return 1;
    }

    /*
     * Generate Huffman codes.
     */
    huffman_code_t codes[256] = {0};

    if (!generate_codes(root, codes))
    {
        printf("Failed to generate Huffman codes\n");
        fclose(input);
        return 1;
    }

    /*
     * Open compressed output file.
     */
    FILE *output = fopen("output.bin", "wb");

    if (output == NULL)
    {
        perror("output.bin");
        fclose(input);
        return 1;
    }

    bit_writer_t writer;

    bit_writer_init(&writer, output);

    /*
     Encode the file we proceed with a byte at a time
     */
    while ((ch = fgetc(input)) != EOF)
    {
        huffman_code_t *code = &codes[(uint8_t)ch];

        if (code->len == 0)
        {
            printf("No Huffman code for byte 0x%02X\n",
                   (uint8_t)ch);

            fclose(input);
            fclose(output);
            return 1;
        }

        if (!bit_writer_write_code(&writer,
                                   code->code,
                                   code->len))
        {
            printf("Failed to write Huffman code\n");

            fclose(input);
            fclose(output);
            return 1;
        }
    }

    /*
     * Flush the final partial byte.
     */
    if (!bit_writer_flush(&writer))
    {
        printf("Failed to flush bit writer\n");

        fclose(input);
        fclose(output);
        return 1;
    }

/*
Now lets display the compression statistics 
           
*/
  long input_size;
long output_size;

/* Get file sizes */
fseek(input, 0, SEEK_END);
input_size = ftell(input);

fseek(output, 0, SEEK_END);
output_size = ftell(output);

printf("\nInput size  : %ld bytes\n", input_size);
printf("Output size : %ld bytes\n", output_size);

if (input_size != 0)
{
    double ratio = (double)output_size / input_size * 100.0;

    printf("Compression ratio : %.2f%%\n", ratio);
}



    fclose(input);
    fclose(output);

    free_tree(root);

    printf("Encoding successful\n");

    return 0;
}