#include <stdio.h>
#include "median.h"
#include "logger.h"


void test_example1() {
    int nums1[] = {1, 3};
    int nums2[] = {2};
    double expected = 2.0;

    INFO("----- Test Example 1 -----");
    INFO("Input nums1 = [1,3], nums2 = [2]");
    INFO("Expected Output = %.2f", expected);

    double result = findMedianSortedArrays(nums1, 2, nums2, 1);

    INFO("Actual Output = %.2f", result);

    if (result == expected)
        INFO("Test Example 1 PASSED");
    else
        ERROR("Test Example 1 FAILED");
}

void test_example2() {
    int nums1[] = {1, 2};
    int nums2[] = {3, 4};
    double expected = 2.5;

    INFO("----- Test Example 2 -----");
    INFO("Input nums1 = [1,2], nums2 = [3,4]");
    INFO("Expected Output = %.2f", expected);

    double result = findMedianSortedArrays(nums1, 2, nums2, 2);

    INFO("Actual Output = %.2f", result);

    if (result == expected)
        INFO("Test Example 2 PASSED");
    else
        ERROR("Test Example 2 FAILED");
}

void test_empty_array() {
    int nums1[] = {};
    int nums2[] = {2, 3};
    double expected = 2.5;

    INFO("----- Test Empty Array -----");
    INFO("Input nums1 = [], nums2 = [2,3]");
    INFO("Expected Output = %.2f", expected);

    double result = findMedianSortedArrays(nums1, 0, nums2, 2);

    INFO("Actual Output = %.2f", result);

    if (result == expected)
        INFO("Test Empty Array PASSED");
    else
        ERROR("Test Empty Array FAILED");
}

void test_single_elements() {
    int nums1[] = {5};
    int nums2[] = {10};
    double expected = 7.5;

    INFO("----- Test Single Elements -----");
    INFO("Input nums1 = [5], nums2 = [10]");
    INFO("Expected Output = %.2f", expected);

    double result = findMedianSortedArrays(nums1, 1, nums2, 1);

    INFO("Actual Output = %.2f", result);

    if (result == expected)
        INFO("Test Single Element PASSED");
    else
        ERROR("Test Single Element FAILED");
}

void test_odd_odd() {
    int nums1[] = {1, 3, 8};
    int nums2[] = {2, 7, 10};
    double expected = 5.0;

    INFO("----- Test Odd + Odd -----");
    INFO("Expected Output = %.2f", expected);

    double result = findMedianSortedArrays(nums1, 3, nums2, 3);

    INFO("Actual Output = %.2f", result);

    if (result == expected)
        INFO("Test Odd+Odd PASSED");
    else
        ERROR("Test Odd+Odd FAILED");
}

void test_even_odd() {
    int nums1[] = {1, 2, 3, 4};
    int nums2[] = {5, 6, 7};
    double expected = 4.0;

    INFO("----- Test Even + Odd -----");
    INFO("Expected Output = %.2f", expected);

    double result = findMedianSortedArrays(nums1, 4, nums2, 3);

    INFO("Actual Output = %.2f", result);

    if (result == expected)
        INFO("Test Even+Odd PASSED");
    else
        ERROR("Test Even+Odd FAILED");
}






int main() {
    INFO("===== Running Median Tests =====");

    test_example1();
    test_example2();
    test_empty_array();
    test_single_elements();
    test_odd_odd();
    test_even_odd();



    return 0;
}
