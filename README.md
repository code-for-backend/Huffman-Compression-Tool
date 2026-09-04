
# File Compressor

Huffman coding implementation in C for lossless data compression.

## Features

* Builds a Huffman tree from byte frequencies
* Generates variable-length Huffman codes
* Supports binary input and output files
* Shows compression statistics 


## Current Status

The core huffman algo is implemented.Planning to add CLI and decompression support in next version

## Example

The current encoder can compress an input file into a binary output stream

```bash
./huff_compressor
```

## Build

Example:

```bash
gcc huffman.c heap.c bit_writer.c huffman_encode_test.c -o huff_compressor
```

##
