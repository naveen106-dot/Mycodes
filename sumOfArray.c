#include <stdio.h>

int* sumOfArray(int arr[],int size,int sum){
    int flag = 0;
    int a[1]; 
    for(int i=0;i<size;i++){
        for(int j=i+1;j<size;j++){
            if (sum == arr[i]+arr[j]){
                a[0] = i;
                a[1] = j;
                flag = 1;
            }
        }
        if (flag == 1){
            return a;
        }
    }
    return 0;
}


int main(){
    int arr[] = {2,4,3,2};
    int sum = 5;
    int size = sizeof(arr) /sizeof(arr[0]);
    printf("size:%d",size,sum);
    int *a = sumOfArray(arr,size,sum);
    for(int i=0;i<5;i++){
    printf("%d",a[i]);
   }
}