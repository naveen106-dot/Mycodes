// reverese the bit in integer constant
//  
#include <stdio.h>

int main()
{
//    int a = 5;
//   char *p = "Hello";
//   //p[0] = 'h';
//   printf("%s", p);
    int val = 0;
    int abc = 0x1234;
    val =  ((abc & 0x000f)<< 12) ; // 4000
                                    //   3000
    val = val | (((abc & 0x00f0)<< 8)>>4); 
    val = val | (((abc & 0x0f00)<< 4)>>8); 
    val = val | (((abc & 0xf000)<< 0)>>12);
     

   //|| ((abc & 0x0f00)<< 8) ||((abc & 0xf000)<< 4);|| ((abc & 0x00f0)<< 8) 

    printf("val :%x\n",val);
    val = -128;
    for(int i=0;i<32;i++){
        if(val&1<<i){
            printf("1");
        }else{
            printf("0");
        }
    }
       // printf("val :%x\n",val);
    printf("\n");
    for(int i=31;0<=i;i--){
        if(val&1<<i){
            printf("1");
        }else{
            printf("0");
        }
    }
    printf("\nval:%d\n",val);
    int num = 0;
    while(val)
    {
        int rem = val % 10;
        printf("rem:%d\n",rem);
        num = (num * 10) + rem;
        printf("num:%d\n",num);
        val = val / 10;
         printf("val:%d\n\n",val);
    }
    printf("\nnum:%d",num);


    return 0;
}
