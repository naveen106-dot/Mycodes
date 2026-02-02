#include <stdio.h>

int main()
{
    int arr[20] = {10,20,30,40,50,60,70,80}; // if you mention size it will calculate total of every size is int = 4 bytes total is 20*4 =80 
    printf("total elements:%d\n",sizeof(arr));
    printf("total size:%d\n",sizeof(arr[0]));
    int size = sizeof(arr)/sizeof(arr[0]);
    printf("size:%d\n",size);
    return 0;
}