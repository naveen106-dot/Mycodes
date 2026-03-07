
#include <stdio.h>

int main()
{
    int arr[5] = {1,2,3,4,5};
    int j = 9;
    arr[j] = 40;
    for(int i=0;i<10;i++){
        printf("%d\n",arr[i]);
    }

    return 0;
}