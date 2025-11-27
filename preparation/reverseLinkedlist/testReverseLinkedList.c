#include "linkedlist.h"

// Helper function
int validateList(Node *head, int *expected, int n) {
    for (int i = 0; i < n; i++) {
        if (!head || head->value != expected[i]) return 0;
        head = head->next;
    }
    return head == NULL;
}

// Test cases
int test_emptyList() {
    Node *head = NULL;
    return reverseLinkedList(head) == NULL;
}

int test_singleNode() {
    int arr[] = {10};
    Node *head = reverseLinkedList(createLinkedListFromArray(arr, 1));
    return (head && head->value == 10 && head->next == NULL);
}

int test_multipleNodes() {
    int arr[] = {1,2,3,4,5};
    int expected[] = {5,4,3,2,1};
    return validateList(reverseLinkedList(createLinkedListFromArray(arr, 5)), expected, 5);
}

int test_negativeValues() {
    int arr[] = {-1,20,-3,4};
    int expected[] = {4,-3,20,-1};
    return validateList(reverseLinkedList(createLinkedListFromArray(arr, 4)), expected, 4);
}
