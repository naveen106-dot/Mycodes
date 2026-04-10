#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num, *arr, i;
    scanf("%d", &num);
    arr = (int*) malloc(num * sizeof(int));
    for(i = 0; i < num; i++) {
        scanf("%d", arr + i);
    }


    // /* Write the logic to reverse the array. */
    // int size = num;
    // for(int j=0;j<num/2;j++){
    //     int temp = arr[size];
    //     arr[size] = arr[j];
    //     arr[j] = temp;
    //     size--;
    //     j++;
    // }

    for(i = 0; i < num; i++)
        printf("%d ", *(arr + i));
    return 0;
}