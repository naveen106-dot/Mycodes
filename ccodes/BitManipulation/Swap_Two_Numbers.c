// Swap two numbers without using a temporary variable.

#include <stdio.h>

int main(){

    int num1 ,num2;

    printf("Enter the two numbers:");
    scanf("%d %d",&num1,&num2);

    num1 = num1 ^ num2;
    num2 = num1 ^ num2;
    num1 = num1 ^ num2;

    printf("num1:%d\n num2:%d\n",num1,num2);

    return 0;
}