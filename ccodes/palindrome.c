// Check if a string is a palindrome — e.g. "madam" → True

#include <stdio.h>
#include <string.h>

int main(){
    char str[100];
    int len=0,i=0,left=0,right=0;
    printf("Enter the string\n");
    scanf("%s",str);

    while(str[i++] != '\0')
        len++;
    printf("len:%d",len);

    right = len - 1;
    int flag = 0;
    while(left < right){
        printf("left :%d ,right:%d\n",left,right);
        if(str[left] != str[right]){
            flag = 1;
        }
        left++;
        right--;
    }
    if(flag == 1){
        printf("it is not a palindrome String:%s",str);
    }else{
        printf("it is a palindrome String:%s",str);
    }
    

}