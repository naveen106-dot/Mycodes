#include <stdio.h>
// 1. find sum of left side and right side 
int main()
{
    int iterate = 10;
    int leftcount = 0;
    int rightcount = 0;
    int arr[] = {1,2,3,7,0};
    int size = sizeof(arr) / sizeof(arr[0]);
    printf("size:%d\n",size);

    int mid = size / 2;
    printf("mid:%d\n",mid);
    
    while(mid >= 0 && mid < size){
        leftcount = 0,rightcount = 0;
        for(int i=0;i< mid;i++){
           leftcount += arr[i];
        }
        for(int j=mid+1;j< size;j++){
            rightcount += arr[j];
        }
        printf("mid :%d leftcount:%d rightcount:%d\n",mid,leftcount,rightcount);
        if(leftcount == rightcount){
            printf("value matched \n");
            return 0;
        }else if(leftcount < rightcount){
            mid++;
        }else{
            mid--;
        }

        if(iterate == 0){
            break;
        }
          iterate--;
    }
    return -1;

}
