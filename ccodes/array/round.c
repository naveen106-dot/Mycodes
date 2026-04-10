#include <stdio.h>

int main()
{
    float num = 46.76;
    unsigned int* val = (unsigned int*)&num;
    for(int i=31;i>=0;i--){
        if(*val & (1 << i)){
            printf("1");
        }else{
            printf("0");
        }
        if(i==31 || i == 23  ){
            printf("  ");
        }
    }
    printf("\n");

    int sign = *val >> 31 &  1 ;

    int exponent = (*val >> 23) & 0xFF;
    int mantisa = *val  & 0X7FFFFF;
    printf("sign:%d\n",sign);
    printf("decimal:%d\n",exponent);
    printf("mantisa:%d\n",mantisa);
    mantisa = 0.1 * mantisa;

    Value = (-1)^sign × 1.mantissa × 2^(exponent)
    
    // float value = (-1)^(sign) * mantisa * 2^(exponent);
    // printf("%f",value);

    
  
}