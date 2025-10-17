#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Type definitions
typedef struct nodeRec {
    int item;
    struct nodeRec* next;
} node;

typedef node* nodePtr;

// Function to create an empty queue
void CreateQueue(nodePtr* head, nodePtr* tail) {
    *head = NULL;
    *tail = NULL;
}

// Function to create a new node
nodePtr CreateNode(int value) {
    nodePtr newNode = (nodePtr)malloc(sizeof(node));
    if (newNode == NULL) return NULL;
    newNode->item = value;
    newNode->next = NULL;
    return newNode;
}

// Function to add an item to the queue
bool Enqueue(nodePtr* head, nodePtr* tail, int value) {
    nodePtr newNode = CreateNode(value);
    if (newNode == NULL) return false;

    if (*tail == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        (*tail)->next = newNode;
        *tail = newNode;
    }
    return true;
}

// Function to remove an item from the queue
bool Dequeue(nodePtr* head, nodePtr* tail) {
    if (*head == NULL) return false;

    nodePtr temp = *head;
    *head = (*head)->next;
    if (*head == NULL) *tail = NULL;
    free(temp);
    return true;
}

// Function to check if the queue is empty
bool IsEmptyQueue(nodePtr head) {
    return head == NULL;
}

// Function to display the queue contents
void ShowQueue(nodePtr head) {
    if (head == NULL) {
        printf("Queue is empty.\n");
        return;
    }

    printf("Queue contents: ");
    nodePtr current = head;
    while (current != NULL) {
        printf("%d ", current->item);
        current = current->next;
    }
    printf("\n");
}

// Function to return the size of the queue
int QueueSize(nodePtr head) {
    if (head == NULL) return 0;

    int count = 0;
    nodePtr current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

#endif
