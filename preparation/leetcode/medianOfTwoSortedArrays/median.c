#include "median.h"
#include "logger.h"

double findMedianSortedArrays(int* nums1, int n1, int* nums2, int n2)
{
    INFO("Finding median of two sorted arrays");

    int total = n1 + n2;
    int merged[total];

    int i = 0, j = 0, k = 0;

    // Merge two sorted arrays
    while (i < n1 && j < n2) {
        if (nums1[i] < nums2[j])
            merged[k++] = nums1[i++];
        else
            merged[k++] = nums2[j++];
    }

    while (i < n1) merged[k++] = nums1[i++];
    while (j < n2) merged[k++] = nums2[j++];

    INFO("Arrays merged successfully");

    // Calculate median
    if (total % 2 == 1)
        return merged[total/2];
    else
        return (merged[total/2 - 1] + merged[total/2]) / 2.0;
}
