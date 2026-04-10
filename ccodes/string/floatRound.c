#include <stdio.h>
#include <string.h>

int main()
{
    float val = 49.7338;
    int num = val;
    printf("num %d\n",num); 

    char str[30];

    sprintf(str,"%f",val);

    printf("str :%s\n",str);

    int size = sizeof(str)/sizeof(str[0]);
    printf("size:%d\n",size);
    int pointval = 0;

    for(int i=0;i<size;i++){

        if(str[i] == '.'){
           int point1 = str[i+1] - '0';
           pointval =  point1;
           printf("pointfirst :%f\n",pointval);

           point1 = 0;

           point1 = str[i+3] - '0';
           printf("point1:%d \n",point1);
           if (point1 <= 5){
             point1 = 0;
             point1 =  str[i+2] - '0';
             printf("less point1:%d\n",point1);
             pointval = pointval * 10 + point1;
            //  int second = (float) point1 / 10.00;
            //  printf("second :%f\n",second);
             
             //pointval = pointval + second;
             printf("value less than 5 :%f\n",pointval);

           }else{
             point1 =  str[i+2] - '0';
             pointval = pointval * 10 + point1;
             pointval++;

             printf("pointval indes greater than 5 :%f\n",pointval);

           }
           
        }
    }
    printf("\n");
    printf("num:%d\n",num);
     printf("pointval:%d\n",pointval);

    float total = num + (float)pointval / 100.00;
    printf("total: %0.2f",total);



}