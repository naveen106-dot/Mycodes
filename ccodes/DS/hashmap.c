#include <stdio.h>

#define SIZE 10

void countNoOfTimeOccurences(int *arr,int size){
    int hash[SIZE] = {0};
    for(int i=0;i<size;i++){
       // printf("%d\n",arr[i]%10);
        int key = arr[i] % SIZE;
        hash[key]++;
    }

    for(int i=0;i<SIZE;i++){
        
            if(hash[i] == 1){
                printf("index:%d\n",i);
            }
           // printf("Value %d occurs %d times\n", i, hash[i]);   
    }

}
int main(){

    int arr[] = {1,2,3,1,2,5,6,9};
    int size = sizeof(arr)/sizeof(arr[0]);
    countNoOfTimeOccurences(arr,size);
   
}