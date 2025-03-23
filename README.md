Huffman Compression & Decompression

Course: Data Structures and Algorithms

Date: 23.3.2025

Introduction
This repository contains a C program for compressing or decompressing a text file using Huffman compression.

Code Overview
The program is divided into two files:

hcompress.c: This file contains the main function and methods for encoding and decoding files using Huffman coding.
linkedList.c and linkedList.h: These files contain the implementation and header for a linked list data structure used in the Huffman tree construction.
Usage
Compilation
Compile the program using "gcc" with all warnings turned on (-Wall flag) on a Linux operating system.

gcc -Wall -o hcompress hcompress.c linkedList.c
Compression
To compress a file:

hcompress -e filename
This creates a compressed file called "filename.huf".

Decompression
To decompress a compressed file:

hcompress -d filename.huf
This creates a decompressed file called "filename.huf.dec".

Huffman Compression/Decompression
The core functionality of the program revolves around Huffman coding, a technique for lossless data compression.

Encoding
greateFreqTable: This method reads a text file and counts all the characters in the file, and returns a table of struct tnodes in an array as the leaf nodes of the Huffman tree.
createHuffmanTree: Constructs the Huffman tree from the frequency table using a linked list.
encodeFile: Encodes a given file using Huffman coding based on the constructed tree.
Decoding
decodeFile: Decodes a compressed file using Huffman coding based on the constructed tree.
These methods allow for efficient compression and decompression of text files while maintaining lossless data integrity.