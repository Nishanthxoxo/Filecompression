#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct tnode {
    char c;
    int freqCount;
    struct tnode *left, *right, *parent;
} tnode;

typedef struct ListNode {
    tnode* treeNode;
    struct ListNode* next;
} ListNode;

tnode* generateFreqTable(char* filename);
tnode* createHuffmanTree(tnode* freqTable);
void encodeFile(char* filename, tnode* leafNodes);
void decodeFile(char* filename, tnode* root);
ListNode* list_add_in_order(ListNode* head, tnode* newNode);
tnode* removeFirst(ListNode** head);

tnode* generateFreqTable(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    tnode* arrHist = calloc(128, sizeof(tnode));
    if (!arrHist) { fclose(file); return NULL; }

    for (int i = 0; i < 128; i++) {
        arrHist[i].c = i;
        arrHist[i].freqCount = 0;
    }

    int temp;
    while ((temp = fgetc(file)) != EOF) arrHist[temp].freqCount++;
    fclose(file);
    return arrHist;
}

tnode* createHuffmanTree(tnode* leafNodes) {
    ListNode* head = NULL;
    for (int i = 0; i < 128; i++) 
        if (leafNodes[i].freqCount > 0) head = list_add_in_order(head, &leafNodes[i]);

    while (head && head->next) {
        tnode* parent = malloc(sizeof(tnode));
        tnode* first = removeFirst(&head);
        tnode* second = removeFirst(&head);

        parent->c = '*';
        parent->freqCount = first->freqCount + second->freqCount;
        parent->left = first; parent->right = second;
        first->parent = parent; second->parent = parent;

        head = list_add_in_order(head, parent);
    }
    return removeFirst(&head);
}

void encodeFile(char* filename, tnode* leafNodes) {
    FILE* inFile = fopen(filename, "r");
    if (!inFile) return;
    FILE* outFile = fopen("output.huf", "wb");
    if (!outFile) { fclose(inFile); return; }

    int currBits[60], bit = 0; unsigned char byte = 0;
    int letter;
    while ((letter = fgetc(inFile)) != EOF) {
        tnode* curr = &leafNodes[letter]; int count = 0;
        memset(currBits, 0, sizeof(currBits));
        while (curr->parent) { currBits[count++] = (curr->parent->right == curr); curr = curr->parent; }
        for (int i = count - 1; i >= 0; i--) {
            byte |= currBits[i] << (7 - bit);
            if (++bit == 8) { fwrite(&byte, 1, 1, outFile); bit = 0; byte = 0; }
        }
    }
    if (bit) fwrite(&byte, 1, 1, outFile);
    fclose(inFile); fclose(outFile);
}

void decodeFile(char* filename, tnode* treeRoot) {
    FILE* file = fopen(filename, "rb");
    if (!file) return;
    FILE* newFile = fopen("decoded.txt", "w");
    if (!newFile) { fclose(file); return; }

    fseek(file, 0, SEEK_END); int byteCount = ftell(file); fseek(file, 0, SEEK_SET);
    tnode* temp = treeRoot; char byte = fgetc(file), currentBit; int bit = 0;
    for (int i = 0; i < byteCount * 8; i++) {
        currentBit = (byte >> (7 - bit)) & 1;
        temp = (currentBit == 0) ? temp->left : temp->right;
        if (++bit == 8) { byte = fgetc(file); bit = 0; }
        if (!temp->left && !temp->right) { fputc(temp->c, newFile); temp = treeRoot; }
    }
    fclose(file); fclose(newFile);
}

ListNode* list_add_in_order(ListNode* head, tnode* newNode) {
    ListNode* newElem = malloc(sizeof(ListNode));
    newElem->treeNode = newNode;
    newElem->next = NULL;

    if (!head || head->treeNode->freqCount >= newNode->freqCount) {
        newElem->next = head;
        return newElem;
    }

    ListNode* current = head;
    while (current->next && current->next->treeNode->freqCount < newNode->freqCount) 
        current = current->next;

    newElem->next = current->next;
    current->next = newElem;
    return head;
}

tnode* removeFirst(ListNode** head) {
    if (!*head) return NULL;
    ListNode* temp = *head;
    tnode* node = temp->treeNode;
    *head = (*head)->next;
    free(temp);
    return node;
}

int main(int argc, char *argv[]) {
    if (argc != 3) return 1;

    tnode* leafNodes = generateFreqTable(argv[2]);
    if (!leafNodes) return 1;
    tnode* treeRoot = createHuffmanTree(leafNodes);
    if (!treeRoot) return 1;

    (strcmp(argv[1], "-e") == 0) ? encodeFile(argv[2], leafNodes) : decodeFile(argv[2], treeRoot);

    free(leafNodes);
    return 0;
}
