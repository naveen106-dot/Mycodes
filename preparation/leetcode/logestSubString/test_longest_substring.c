#include <stdio.h>
#include <string.h>
#include "longest_substring.h"

int tests_passed = 0;
int tests_failed = 0;

// Simple test function
void run_test(char *input, int expected) {
    int output = lengthOfLongestSubstring(input);

    if (output == expected) {
        printf("[PASS] Input: \"%s\"  → Output: %d\n", input, output);
        tests_passed++;
    } else {
        printf("[FAIL] Input: \"%s\"  → Output: %d (Expected %d)\n",
               input, output, expected);
        tests_failed++;
    }
}

int main() {
    printf("Running Tests for lengthOfLongestSubstring:\n\n");

    run_test("abcabcbb", 3);
    run_test("bbbbb", 1);
    run_test("pwwkew", 3);
    run_test("", 0);
    run_test("abcdef", 6);
    run_test("aab", 2);

    printf("\nTests Passed: %d\n", tests_passed);
    printf("Tests Failed: %d\n", tests_failed);

    return 0;
}
