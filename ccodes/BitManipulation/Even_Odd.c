// Check if a number is even or odd using bitwise operator.

#include <stdio.h>

int main(){

    int num;

    printf ("Enter the number:");
    scanf("%d",&num);

    if (num  & 1){
        printf("odd\n");
    }else{
        printf("even\n");
    }
    
    
    return 0;
}