#include <stdio.h>

int binarySearch(int arr[],int len){
    int left = 0;
    int right = len-1;
    int find = 6;
    while(left < right){
        int mid = (left + right)/2 ;
        printf("mid:%d\n",mid);
        if(arr[mid] == find){
            printf("yes\n");
            return 1;
        }else if(arr[mid] < find){
            left = mid;
        }else{
            right = mid;
        }
    }
}
int findMin(int arr[]){
    int len = 5;
    printf("length:%d\n",len);
    int min=arr[0];
    int val=0;

    for(int i=1;i<len;i++){
        int val = arr[i];
        if(!(min < val)  ){
            min = val;
            //printf("min:%d\n");
        }
    }
    printf("total-min:%d\n",min);
    return min;
}

int main(){
    int arr[] = {5,6,7,8,9,10,11,12,13};
    //findMin(arr);
    binarySearch(arr,9);
}