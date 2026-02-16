
/* Input:
arr = {1, 2, 3, 4, 5}
K = 2

Output:
{4, 5, 1, 2, 3} */

#include <stdio.h>

int main()
{
    int arr[] = {1,2,3,4,5};
    int k = 2;
    int size = sizeof(arr)/sizeof(arr[0]);
    
    printf("len:%d\n",size);
    for(int i=0;i<k;i++){
        int temp = arr[size-1];
        printf("temp :%d",temp);
        for(int j= size-1;j>=0;j--){
            arr[j]=arr[j-1];
        }
        arr[0] = temp;
    }
    
    for(int i=0;i<size;i++){
        printf("%d",arr[i]);
    }

    return 0;
}