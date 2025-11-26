#include "sumheader.h"

void generateTestArray(int *arr, int n, int *missing) {
    *missing = (rand() % n) + 1;

    int index = 0;
    for (int i = 1; i <= n; i++) {
        if (i == *missing) continue;
        arr[index++] = i;
    }
}

void printArray(int *arr, int n) {
    printf("[ ");
    for (int i = 0; i < n - 1; i++)
        printf("%d ", arr[i]);
    printf("]\n");
}

void testMissing(int testNo, int n) {
    printf("\n=============================\n");
    printf("TEST %d\n", testNo);

    int arr[100];
    int missing;

    generateTestArray(arr, n, &missing);

    printf("Expected missing number: %d\n", missing);
    printf("Array: ");
    printArray(arr, n);

    int result = findMissingNum(arr, n);

    printf("Result: %d\n", result);

    if (result == missing)
        printf("TEST PASSED ✔\n");
    else
        printf("TEST FAILED ❌\n");

    printf("=============================\n");
}