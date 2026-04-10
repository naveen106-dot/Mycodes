#include <stdio.h>

int factorial(int num){
    //static int result;
    printf("num:%d\n",num);
    if(num ==0){
        return 1;
    }else{
        return num * factorial(num -1);
    }

}

int cal_nth_term(int n,int a,int b,int c){
  
    static int total;
    static int val = 4;

    printf("total:%d\n",total);
    if(n != 5){
        return 1;
    }else{
        a = b;
        b = c;
        c = n;

        return total + cal_nth_term(n-1,a,b,c);
    }

}

int main(){
    int n=5;
    int a = 1,b = 2,c = 3;
    int total = cal_nth_term(n,a,b,c);
    //int total = factorial(n);
    printf(" totaldata :%d",total);

    return 0;
}