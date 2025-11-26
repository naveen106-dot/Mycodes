#include "linkedlist.h"
#include <stdio.h>

// Declare test functions here
int test_emptyList();
int test_singleNode();
int test_multipleNodes();
int test_negativeValues();

// Implementation of run_test()
void run_test(int (*test_func)(), const char *test_name, int *pass_count) {
    printf("Running %s... ", test_name);
    if (test_func()) {
        printf("✔ PASSED\n");
        (*pass_count)++;
    } else {
        printf("❌ FAILED\n");
    }
}

int main() {
    int pass_count = 0;
    int total_tests = 4;

    printf("\n======= Linked List Reverse Function Tests =======\n\n");

    run_test(test_emptyList, "test_emptyList", &pass_count);
    run_test(test_singleNode, "test_singleNode", &pass_count);
    run_test(test_multipleNodes, "test_multipleNodes", &pass_count);
    run_test(test_negativeValues, "test_negativeValues", &pass_count);

    printf("\n=================================================\n");
    printf("Total Tests: %d | Passed: %d | Failed: %d\n",
           total_tests, pass_count, total_tests - pass_count);
    printf("=================================================\n");

    return 0;
}
