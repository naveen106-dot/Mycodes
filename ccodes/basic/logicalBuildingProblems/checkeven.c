#include <stdio.h>

int iseven(int num)
{
    return num%2;
}


int main(){

    int num = 12;
    if(iseven(num)){
        printf("It is odd number");
    }else{
        printf("It is even number");
    }
}