#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    struct node *next;
} Node;

// Core linked list functions
Node* createLinkedListFromArray(int *arr, int n);
void printLinkedList(Node *head);
Node* reverseLinkedList(Node *head);

// Declare run_test() here
void run_test(int (*test_func)(), const char *test_name, int *pass_count);

#endif
