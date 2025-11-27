/* Example 1:
Input: s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3. Note that "bca" and "cab" are also correct answers.*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int lengthOfLongestSubstring(char* s) {
    int len = strlen(s);
    int max_count =0;
    char subStr[20];
    int index =0,flag = 0;
    printf("len:%d",len);
    int start,end;
    for(start =0;s[start] <= len;start++){
         for(int i=start;i<len;i++){
            char ch = s[i];
            for(int j=0;j<strlen(subStr);j++){
                if(subStr[j] != ch ){
                    flag = 1;
                }
            }
            if(flag == 1){
                subStr[index] = char;
            }else{
                subStr = '\0';
            }
         }
    }
}

int main(){
    char str[100];
    printf("Enter the string:\n");
    scanf("%s",str);
    char* s = str; 
    lengthOfLongestSubstring(s);


    return 0;
}