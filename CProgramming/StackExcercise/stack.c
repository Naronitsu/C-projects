#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Create an empty stack (NULL top)
nodePtr createStack(void) {
    return NULL;
}

// Destroy all nodes and reset top to NULL
void destroyStack(nodePtr *top) {
    if (!top) return;
    clearStack(top);
}

// Push an item onto the stack
bool push(nodePtr *top, int value) {
    nodePtr newNode = malloc(sizeof(node));
    if (!newNode) return false;

    newNode->item = value;
    newNode->next = *top;
    *top = newNode;
    return true;
}

// Pop an item from the stack
bool pop(nodePtr *top, int *outValue) {
    if (!top || !*top) return false;

    nodePtr temp = *top;
    if (outValue) *outValue = temp->item;
    *top = temp->next;
    free(temp);
    return true;
}

// Peek at the top value without removing it
bool peek(nodePtr top, int *outValue) {
    if (!top) return false;
    if (outValue) *outValue = top->item;
    return true;
}

// Check if stack is empty
bool isEmpty(nodePtr top) {
    return top == NULL;
}

// Count elements in the stack
int stackSize(nodePtr top) {
    int count = 0;
    while (top) {
        count++;
        top = top->next;
    }
    return count;
}

// Clear all nodes, keeping the stack usable
void clearStack(nodePtr *top) {
    int dummy;
    while (pop(top, &dummy));
}

// Print contents of stack (top to bottom)
void printStack(nodePtr top) {
    printf("Stack: ");
    if (!top) {
        printf("[empty]\n");
        return;
    }

    while (top) {
        printf("%d ", top->item);
        top = top->next;
    }
    printf("\n");
}
