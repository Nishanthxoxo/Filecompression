#include "linkedList.c"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

struct tnode* generateFreqTable(char* filename);
struct tnode* createHuffmanTree(struct tnode* freqTable);
void encodeFile(char* filename, struct tnode* leafnodes);
void decodeFile(char* filename, struct tnode* root);

int main(int argc, char *argv[]) {

  // Check the make sure the input parameters are correct

   if (argc != 3) {
      printf("Error: The correct format is \"hcompress -e filename\" or \"hcompress -d filename.huf\"\n"); fflush(stdout);

    exit(1);

  }

  // Create the frequency table by reading the generic file

  struct tnode* leafNodes = generateFreqTable("decind.txt");

  // Create the huffman tree from the frequency table

  struct tnode* treeRoot = createHuffmanTree(leafNodes);

  // encode

  if (strcmp(argv[1], "-e") == 0) {

    // Pass the leafNodes since it will process bottom up

    encodeFile(argv[2], leafNodes);

  } else { // decode

    // Pass the tree root since it will process top down

    decodeFile(argv[2], treeRoot);

  }

  return 0;

}
 tnode* generateFreqTable(char* filename){
   // opening file
   FILE* file = fopen(filename, "r");

   //allocating array of 128 tnodes
   tnode* arrHist = (tnode*)malloc(128*sizeof(tnode));
   //setting coresseponding ascii code 
   for(int i = 0; i<128; i++){
     (arrHist+i)->c = i;
     (arrHist+i)->freqCount = 0;
   }
   //reading character
   int temp = fgetc(file);
   while(temp != -1){
     (arrHist+temp)->freqCount++;
     temp = fgetc(file);
   }
   return arrHist;
 }

tnode* createHuffmanTree(tnode* leafNode){

  //linked list is made to store nodes of huffman tree
  LinkedList* ll = llCreate();
  //leaf node is added to link lost in order of increasing freq count
  for (int p = 0; p < 128; p++) {
    list_add_in_order(&ll, &leafNode[p]);
  }
  while(ll->next != NULL){
    tnode* parent = (tnode*)malloc(1*sizeof(tnode));
    tnode* first = removeFirst(&ll);
    tnode* second = removeFirst(&ll);
    //parent node created with sum of two children
    int sum = ((first->freqCount)+(second->freqCount));
    parent->c = '*';
    parent->freqCount = sum;
    parent->left = first;
    parent->right = second;
    first->parent = parent;
    second->parent = parent;
    //adding parent back into list in order
    list_add_in_order(&ll, parent);
  }
  tnode* root = removeFirst(&ll);
  return root;
}


// Encode the input file using Huffman coding and write the output to a binary file
void encodeFile(char* filename, struct tnode* leafNode) {
  FILE* inFile = fopen(filename, "r");
  if (inFile == NULL) {
    perror("Error opening input file");
    exit(EXIT_FAILURE);
  }
  //creating output file
  FILE* outFile = fopen("decind.txt.huf", "wb");
  if (outFile == NULL) {
    perror("Error opening output file");
    exit(EXIT_FAILURE);
  }

  //variables to hold the binary code
  int currBits[60];
  int bit = 0;
  unsigned char byte = 0;
  int letter = fgetc(inFile);
  while(letter != EOF) {
    struct tnode* curr = &leafNode[letter];
    int count = 0;
    memset(currBits, 0, sizeof(currBits)); // Initialize currBits to all zeros
    while(curr->parent != NULL) {
      if(curr->parent->left == curr) {
        currBits[count] = 0;
      } else {
        currBits[count] = 1;
      }
      count++;
      curr = curr->parent;
    }
    for(int i = count - 1; i >= 0; i--) {
      byte |= currBits[i] << (7 - bit);
      bit++;
      if(bit == 8) {
        fwrite(&byte, sizeof(unsigned char), 1, outFile);
        bit = 0;
        byte = 0;
      }
    }
    letter = fgetc(inFile);
  }
  if(bit != 0) {
    fwrite(&byte, sizeof(unsigned char), 1, outFile);
  }

  fclose(inFile);
  fclose(outFile);
}

//takes filename and root tnode as parameters
//decodes the huffman-encoded file
void decodeFile(char* filename, struct tnode* treeRoot){
  struct tnode* temp = treeRoot; // temp pointer to root node. traverse tree while decoding file
    FILE* file = fopen(filename,"rb");
    char byte = fgetc(file);
    char currentBit = 0;
    int bit = 0;
    FILE* newFile = fopen("decind.huf.txt", "w");
    fseek(file, 0, SEEK_END);//moves file pointer to end of file to get total # of bytes in file
    int byteCount = ftell(file); //total # of bytes in file
    fseek(file, 0, SEEK_SET);//moves pointer back to beginning of file

    for(int i=0; i < byteCount * 8; i++){
        currentBit = (byte >> (7-bit)) & 1;
        bit++;
        if(currentBit == 0){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
        if(bit == 8){
            byte = fgetc(file);
            bit = 0;
        }
        if(temp->left == NULL && temp->right == NULL){
            fputc(temp->c, newFile);
            temp = treeRoot;
        }
    }
}