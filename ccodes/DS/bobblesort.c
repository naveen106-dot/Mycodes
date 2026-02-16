#include <stdio.h>

int main()
{
    int num = 10;
    int count = 0;

    while(num){
         if(num & 1){
            num = num >> 1;
            count = count +1 ;
        }
    }
    if(count == 1){
        printf("power of 2");
    }
    else{
        printf("it is not power of 2");
    }
}