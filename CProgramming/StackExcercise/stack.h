#ifndef STACK_H
#define STACK_H

#include <stdbool.h>   // for bool

// Singly-linked node used by the stack
typedef struct nodeRec {
    int item;
    struct nodeRec *next;
} node;

typedef node* nodePtr;   // The stack “top” is a nodePtr (NULL means empty)

// --- Construction / destruction ---
nodePtr createStack(void);                 // returns NULL (empty stack)
void    destroyStack(nodePtr *top);        // frees all nodes and sets *top = NULL

// --- Core operations ---
bool    push(nodePtr *top, int value);     // returns false on allocation failure
bool    pop(nodePtr *top, int *outValue);  // returns false if empty
bool    peek(nodePtr top, int *outValue);  // returns false if empty
bool    isEmpty(nodePtr top);

// --- Utilities ---
int     stackSize(nodePtr top);
void    clearStack(nodePtr *top);          // remove all elements (like destroy but keeps stack usable)
void    printStack(nodePtr top);           // prints contents; implemented in .c (uses stdio)

#endif // STACK_H
