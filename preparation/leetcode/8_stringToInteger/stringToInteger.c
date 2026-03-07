/* Example 1:

Input: s = "42"

Output: 42

Explanation:

The underlined characters are what is read in and the caret is the current reader position.
Step 1: "42" (no characters read because there is no leading whitespace)
         ^
Step 2: "42" (no characters read because there is neither a '-' nor '+')
         ^
Step 3: "42" ("42" is read in)
           ^

Example 2:

Input: s = " -042"

Output: -42

Explanation:

Step 1: "   -042" (leading whitespace is read and ignored)
            ^
Step 2: "   -042" ('-' is read, so the result should be negative)
             ^
Step 3: "   -042" ("042" is read in, leading zeros ignored in the result)
               ^

Example 3:

Input: s = "1337c0d3"

Output: 1337

Explanation:

Step 1: "1337c0d3" (no characters read because there is no leading whitespace)
         ^
Step 2: "1337c0d3" (no characters read because there is neither a '-' nor '+')
         ^
Step 3: "1337c0d3" ("1337" is read in; reading stops because the next character is a non-digit)*/


#include <stdio.h>
#include <stdlib.h>
#define MIN -(1<<31)
#define MAX (1<<31)-1
int myAtoi(char* s) {

    int num = 0;
    int len = 0;
    int flag = 0;
    while(s[len] != '\0'){
        len++;
    }
    // int sign = (s > 0) ? -1 : 1;

    for(int i=0;i<len;i++){
        int val = s[i] - '0';
        if((s[i] < '0' || s[i] > '9')){
            break;
        }
         
        if(s[i] == '0'){
            int t=1;
                 if(s[t] == '-'){
                        return 0;
                 }
             
             }
        

        if('-' == s[i]){
            flag = 1;
            continue;
        }else if(' ' == s[i]  || '_' == s[i]){
            continue;
        }else if((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')){
            
            break;
        }
        
      
        num = num * 10 + val; 
    
    }
    if(flag == 1){
        num = -num;
    }
  

    return num;
}

int main()
{
    //char s[] = "___  -042";
    //char s[] = "1337c0d3";
    char s[] = "-042";
    myAtoi(s);

}