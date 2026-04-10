#include <stdio.h>

void printtable(int num)
{
    for (int i=10;i>0;i--){
        printf("%d * %d = %d\n",num,i,num*i);
    }
}

void sumOfNaturals(int num)
{
    int i = num;
    int total = 0;
    while(i)
    {
        total += i;
        i--;
    }
    printf("total :%d",total);
}

int main()
{
    // int table = 11;
    // printtable(table);
    int num = 10;
    sumOfNaturals( num);

}