#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

typedef struct tnode{
  int freqCount;
  int c;
  struct tnode* left;
  struct tnode* right;
  struct tnode* parent;
}tnode;


typedef struct node {
  tnode* value;
  struct node* next;
} LinkedList;

LinkedList* llCreate ();
int llIsEmpty(LinkedList* ll);
void llDisplay(LinkedList* ll);
void llAdd(LinkedList** ll, tnode* newValue);
void llFree(LinkedList* ll);
void list_add_in_order (LinkedList** ll, tnode* newValue);
tnode* removeFirst(LinkedList** ll);