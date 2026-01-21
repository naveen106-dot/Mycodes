#include <stdio.h>
#include <string.h>

void findGretestPalindrome(char *str){
    printf("string: %s\n",str);
    
}



int main(){
    char string[100];
    strcpy(string,"i am ama | i speak malayalam | i like racecar");
   // printf("%s\n",string);
    findGretestPalindrome(string);
}