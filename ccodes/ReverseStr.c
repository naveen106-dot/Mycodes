// Reverse a string — e.g. "hello" → "olleh"

#include <stdio.h>

int main(){

    char str[15] = "hello";
    int len = 0,i=0;
    while(str[i] != '\0'){
        printf("%c",str[i]);
        i++;
        len++;
    }
    printf("\nlen:%d\n",len);
    int left = 0;
    int right = len - 1;
    while(left < right){
        // printf("\n len:%d\n",len);
        char ch = str[left];
        str[left++] = str[right];
        str[right--] = ch;
    }
    printf("\nAfter Reverse of String:%s",str);
    return 0;
}