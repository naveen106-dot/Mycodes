#include <time.h>
#include "SumHeader.h"


int main() {

     srand(time(NULL));

    for (int i = 1; i <= 30; i++) {
        testMissing(i, 10);
    }

    // int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 10}; // missing 9
    // int n = 10;

    // int missing = findMissingNum(arr, n);
    // printf("Missing number is: %d\n", missing);

    return 0;
}
