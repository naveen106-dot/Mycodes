#include <stdio.h>


int binarySearch(int arr,int search)
{
    int len = sizeof(arr)/sizeof(arr[0]);
    int left = 0,right = len;
    int mid = left + right / 2;
    printf("mid : %d\n",mid);
    while(left <= right){
        if (arr[mid] <= search ){

        }else if(arr[mid] >= search){

        }
    }

}


int main()
{
    int arr[] = {1,2,3,4,5,6,7,8,9,10};
    int search = 7;
}