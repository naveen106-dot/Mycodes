#include <stdio.h>

int main(){
    int arr[] = {1,2,3,1,2,5,6,9};
    int size = 8;
    int arrCopy[size];
    for(int i=0;i<size;i++){
        printf("%d\n",arr[i]%10);
    }
}