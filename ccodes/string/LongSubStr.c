/*Example 1:
Input: s = "abcabcbb"
Output: 3
Explanation: The answer is "abc", with the length of 3.
Example 2:

Input: s = "bbbbb"
Output: 1
Explanation: The answer is "b", with the length of 1.
Example 3:

Input: s = "pwwkew"
Output: 3
Explanation: The answer is "wke", with the length of 3.
Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.
*/
 
#include <stdio.h>

int main(){
    int i,j;
    char *input =  "abcabcbb",*set[128] = 0;

 for(i=0;input[i] == 0;i++){
    for(j=i;input[j] == 0;j++){
        if(set[j] != 0){

        }else{
            break;
        }
    }
 }

 for (int i=0;)

}


#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function to check if all characters in substring are unique
bool allUnique(char *s, int start, int end) {
    int seen[256] = {0};   // ASCII characters

    for (int i = start; i <= end; i++) {
        if (seen[(unsigned char)s[i]]) {
            return false;   // already seen
        }
        seen[(unsigned char)s[i]] = 1;
    }
    return true;
}

// Brute-force function
int lengthOfLongestSubstring(char *s) {
    int n = strlen(s);
    int maxLen = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            if (allUnique(s, i, j)) {
                int len = j - i + 1;
                if (len > maxLen) maxLen = len;
            }
        }
    }
    return maxLen;
}

int main() {
    char s1[] = "abcabcbb";
    char s2[] = "bbbbb";
    char s3[] = "pwwkew";

    printf("Input: %s, Output: %d\n", s1, lengthOfLongestSubstring(s1));
    printf("Input: %s, Output: %d\n", s2, lengthOfLongestSubstring(s2));
    printf("Input: %s, Output: %d\n", s3, lengthOfLongestSubstring(s3));

    return 0;
}
