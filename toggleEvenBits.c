// write a c program for toggle even bits
#include <stdio.h>

int main() {
    int value = 10,i;
    for(i=0;i<32;i++){
        if(i%2 == 0){
            value = value ^ ( 1 << i); 
        }
    }
    
    for(int j=31;j>=0;j--){
        printf("%d", (value >> j) & 1);
    }
    return 0;
}