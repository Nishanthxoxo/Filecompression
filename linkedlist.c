#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
  
LinkedList* llCreate() {
  return NULL;
}
 
int llIsEmpty(LinkedList* ll) {
  return (ll == NULL);
}
void llDisplay(LinkedList* ll) {
 
  LinkedList* p = ll;
  printf("[");
 
  while (p != NULL) {
    printf("%d, ", p ->value->freqCount);
    p = p->next;
  }
  printf("]\n");
}
 
void llAdd(LinkedList** ll,  tnode* newValue) {
   LinkedList* newNode = (LinkedList*)malloc(1 * sizeof(LinkedList));
  newNode->value = newValue;
  newNode->next = NULL;
 
  int value  = newValue -> freqCount;
 
  LinkedList* p = *ll;
  if (p == NULL) {  
    *ll = newNode;  
  } else {
    while (p->next != NULL) {
      p = p->next;
    }
   
    p->next = newNode;
  }
}
void list_add_in_order (LinkedList** ll, tnode* newValue) {
  LinkedList* newNode = (LinkedList*)malloc(1 * sizeof(LinkedList));
 
  newNode->value = newValue;
  newNode->next = NULL;
 
  int valComparator = newValue->freqCount;

  if (*ll == NULL) {
    *ll = newNode;
    (*ll)->next = NULL;

  }
  else if((*ll)->value->freqCount > valComparator) {

    newNode->next = (*ll);
    (*ll) = newNode;
  
  }
  else {
    LinkedList* temp = *ll;
    while( temp->next != NULL && valComparator >= temp->next->value->freqCount) {
	temp = temp->next;
      }

      newNode->next = temp->next;
      temp->next = newNode;    
  }

}
 
 tnode* removeFirst(LinkedList** ll) {
   LinkedList* curr = *ll;
   if (llIsEmpty(*ll)){
       return NULL;
  }
 
  tnode* temp = curr->value;
 
  (*ll) = (*ll) -> next;
   
 return temp;
}
 
 
void llFree(LinkedList* ll) {
 
  LinkedList* p = ll;
  while (p != NULL) {
    LinkedList* oldP = p;
    p = p->next;
    free(oldP);
  }
 
}
 