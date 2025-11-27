#include "linkedlist.h"
#include <stdio.h>

// Create linked list from array
Node* createLinkedListFromArray(int *arr, int n){
    Node *head = NULL, *temp = NULL;

    for(int i=0; i<n; i++){
        Node *newNode = malloc(sizeof(Node));
        newNode->value = arr[i];
        newNode->next = NULL;

        if(head == NULL){
            head = newNode;
            temp = head;
        } else {
            temp->next = newNode;
            temp = newNode;
        }
    }
    return head;
}

// Print linked list
void printLinkedList(Node *head){
    Node *temp = head;
    while(temp){
        printf("%d ", temp->value);
        temp = temp->next;
    }
    printf("\n");
}

// Reverse linked list
Node* reverseLinkedList(Node *head){
    Node *prev = NULL, *curr = head, *next = NULL;
    while(curr){
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}


