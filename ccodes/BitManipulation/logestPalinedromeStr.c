/* #include <stdio.h>

int len(char *s1){
    int i =0,count=0;
    while(s1[i] != '\0'){
        count++;
        i++;
    }
    return count;
}

char* longestPalindrome(char* s) {
    int left=0;
    int right = len(s)-1;
    printf("length:%d\n",right);
    int flag = 0;
    char res[100] = {0},index = 0;
    
    
    while (left<=right)
    {
        printf("s[%d]:%c s[%d]:%c\n",left,s[left],right,s[right]);
        if(s[left] == s[right])
        { 
            flag = 1;  
            
        }else{
            flag = 0;
        }
        if(flag == 1 ){
            flag = 0;
            res[index++] = s[left];
        }
        left++;
        right--;
    }
    printf("res:%s\n",res);

    int size = len(res);
    printf("size:%d\n",size);
    
    if(left+right % 2 == 0){

        for(int i=size-2;i>=0;i--){
        char ch = res[i];

        res[index++] = ch;
    }

    }else{
        for(int i=size;i>=0;i--){
        char ch = res[i];

        res[index++] = ch;
    }
    }

    printf("after attached:%s",res);
    char *string = res;

    return string;
}
int main(){

    char str[] = "babad";
    //char str[] = "cbbd";
    char *s = str;

    longestPalindrome( s);
}*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int expand(char *s, int left, int right) {
    int len = strlen(s);
    printf("left:%d right:%d\n",left,right);
    while (left >= 0 && right < len && s[left] == s[right]) {
        left--;
        right++;
    }
    return right - left - 1;   // length of palindrome
}

char* longestPalindrome(char* s) {
    int len = strlen(s);
    if (len == 0) return "";
    printf("len:%d",len);

    int start = 0, end = 0;

    for (int i = 0; i < len; i++) {
        int len1 = expand(s, i, i);       // odd length
        int len2 = expand(s, i, i + 1);   // even length

        int maxLen = (len1 > len2) ? len1 : len2;

        if (maxLen > end - start) {
            start = i - (maxLen - 1) / 2;
            end   = i + maxLen / 2;
        }
    }

    int result_len = end - start + 1;
    char *res = malloc(result_len + 1);     // allocate memory
    strncpy(res, s + start, result_len);
    res[result_len] = '\0';

    return res;
}

int main() {
    char str1[] = "babad";
    char str2[] = "cbbd";

    printf("%s → %s\n", str1, longestPalindrome(str1)); // "bab" or "aba"
    printf("%s → %s\n", str2, longestPalindrome(str2)); // "bb"

    return 0;
}
