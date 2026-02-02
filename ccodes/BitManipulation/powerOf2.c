#include <stdio.h>
#include <math.h>

void power(int num)
{
     int a = 1;
    printf("%d\n",  1 << num);

}
void changebits(int num)
{
    printf("hello\n");
    int val = ((num & 0x000F) << 12 ) |  ( (num & 0XFFF0) >> 4)  ;
    printf("%X\n",val);

}

void countSetBits(int num)
{
  // method1
  int count = 0;
  for(int i=31;i>=0;i--){
    if(num & (1 << i)){
      count++;
    }
  }
  printf("count:%d\n",count);

  // method2

  int count1 = 0;
  while(num){
    num = num & (num - 1);
    count1++;
    printf("num:%d\n",num);
  }
  printf("count1:%d",count1);

}


// void countSetBits(unsigned int x)
// {
//     int count = 0;
//     int i = 0;

//     while (x) {
//         if (x & 1)
//             count++;
//         x >>= 1;
//       i++;
//     }
//     printf("i:%d",i);
//     return count;
// }


// void firstSetBitPos(int num)
// {
//   int pos ;
//   for(int i=0;i<=31;i++){
//     if(num & 1 << i){
//       pos = i;
//       break;
//     }
//   }
//   printf("pos:%d",pos);
// }
int leastSetBitPos(unsigned int x)
{
    if (x == 0)
        return -1;

    unsigned int lsb = x & (~x + 1);   // or x & -x
    return log2(lsb);
}
void swapOddEvenOnes(int num)
{
  

}

int main()
{
    __u_int input = 8;
    //changebits(input);
    int val = 0XA1;
    int set = val | (1 << 1);
    printf("%x\n",set);
    int clear = val & ~(1 << 1);
    printf("%x\n",clear);
    int toggle = val & ( 1 << 0);
    printf("toggle:%d\n",toggle);
    //countSetBits(input);
    //firstSetBitPos(input);
    leastSetBitPos(input);
    swapOddEvenOnes(input);
    
}