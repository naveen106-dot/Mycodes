#include <stdio.h>

#define INT_MIN -(1 << 31)
#define INT_MAX  (1 << 31)-1

int reverse(int x){

    int num = 0;
    printf("INT_MIN:%d\n",INT_MIN);
    printf("INT_MAX:%d\n",INT_MAX);
    while(x != 0){
        int rem = x % 10;
        if(num > INT_MIN /10 || num == INT_MIN /10 && rem < -8  ){
            return 0;
        }

        if(num < INT_MAX /10 || num == INT_MAX / 10 && rem > 7 ){
            return 0;
        }

        num = num * 10 + rem;
        x = x / 10;

    }
    return num;
}

int main()
{
    int num = -123;
    int rev = reverse(num);
    printf("%d",rev);
}