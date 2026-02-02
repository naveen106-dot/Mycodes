#include <stdio.h>

void toggleBit(int num)
{
    num =  num ^ (1 << 0);
    printf("%d",num);
}
void  binary(int num)
{
    printf("%b",num);
}
void eightmul(void)
{
    for (int i=0;i<=10;i++){
        int val = (i << 3);
        printf("val:%d\n",val); 
    }
}
void powerOf2(int num)
{
    if(num & (num-1))
    {
        printf("it is not a power of 2");
    }else{
        printf("it is power of 2");
    }
}
void powerOf4(int num){
    for(int i=31;i>0;i--){
        if(i%2 == 0){
            if(num & 1 << i){
                num = num ^ (1 << i);
                printf("num:%d\n",num);
                break;
            }
        }
    }
            if(num == 0){
                printf("it is power of 4 \n");
            }else{
                printf("it is not power of 4\n");
            }
}

void powerOf8(int num)
{

}

int main()
{
    int val = 10;
    int val1 = 0b1010;
    //eightmul();
   // powerOf2(16);
    // powerOf4(8);
    powerOf8();
    //binary(val1);
    //toggleBit(val);
}