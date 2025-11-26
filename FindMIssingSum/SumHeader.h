#ifndef SUMHEADER_H
#define SUMHEADER_H

#include <stdio.h>
#include <stdlib.h>

int findMissingNum(int a[], int n);


void generateTestArray(int *arr, int n, int *missing);
void printArray(int *arr, int n);
void testMissing(int testNo, int n);

#endif