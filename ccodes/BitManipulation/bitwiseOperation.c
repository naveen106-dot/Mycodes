#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//Complete the following function.


void calculate_the_maximum(int n, int k) {
  //Write your code here.
  int andMax =0,orMax=0,xorMax=0;
  printf("function called \n");

    for(int i=1;i<n;i++){
        for(int j=i;j<=k;j++){
            printf("i:%d j:%d",i,j);
            int andOper = i & j;
            if(andOper < k){
                if(andOper > andMax){
                    andMax = andOper;
                    printf("andMax: %d  ",andMax);
                }
            }
            int orOper = i | j;
             if(orOper < k){
                if(orOper > orMax){
                        orMax = orOper;
                }
            }
            int xorOper = i ^ j;
             if(xorOper < k){
                if(xorOper > xorMax){
                        xorMax = xorOper;
                }
            }
        }
    }
    printf("\nandMax %d\n",andMax);
    printf("orMax  %d\n",orMax);
    printf("xorMax %d\n",xorMax);
   
}
int main() {
    int n, k;
  
    scanf("%d %d", &n, &k);
    calculate_the_maximum(n, k);
 
    return 0;
}
