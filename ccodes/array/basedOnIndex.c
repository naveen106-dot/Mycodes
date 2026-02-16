#include <stdio.h>


int main()
{
    int arr[] = {1,2,3,4,5};
    int size = sizeof(arr)/sizeof(arr[0]);

    int num = 27;
    printf("size:%d\n",size);
    if(num >size){
        num = num % 5;
    }
    printf("num:%d\n",num);

    for(int i=0;i<=5;i++)
    {
        if(num == 5){
            num = 0;
        }else{
            printf("%d  ",arr[num++]);
        }   
    }
}