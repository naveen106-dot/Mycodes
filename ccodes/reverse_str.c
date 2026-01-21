#include <stdio.h>

/*count the string length*/
int countStringLength(char str[]){
    printf("countStringLength called ");
    int i=0,count = 0;
    while(str[i] != 0){
        count++;
    }
    printf("count :%d",count);
    return count;

}

void reverse_str(char *str){
    int count =  countStringLength(str);
    printf("count:%d\n",count);
    int i=0,j=count;
    while(i<j){
        char ch = str[i];
        str[i] = str[j];
        str[j] = ch; 
        i++;
        j--;
    }
    printf("string :%s",str);

}

int main(){
    char input[] = "programming";
    reverse_str(input);

}