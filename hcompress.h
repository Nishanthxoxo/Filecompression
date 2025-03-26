
#include "linkedlist.h"

struct tnode* generateFreqTable(char* filename);
struct tnode* createHuffmanTree(struct tnode* freqTable);
void encodeFile(char* filename, struct tnode* leafnodes);
void decodeFile(char* filename, struct tnode* root);
