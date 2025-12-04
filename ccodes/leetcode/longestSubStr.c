#include <stdio.h>
#include <string.h>

char charArr[100] = {0};

int lengthOfLongestSubstring(char* s) {
    int n = strlen(s);
    int maxlen = 0, flag = 0, index = 0;

    for (int i = 0; i < n; i++) {
        index = 0; 
        charArr[index++] = s[i];
        for (int j = i + 1; j < n; j++) {
            flag = 0;
            for (int k = 0; k < index; k++) {
                if (s[j] == charArr[k]) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                charArr[index++] = s[j];
            } else {
                break;
            }
        }
        // update max length AFTER finishing substring
        if (index > maxlen) {
            maxlen = index;
        }

        // clear charArr only AFTER inner loop
        for (int l = 0; l < index; l++) {
            charArr[l] = 0;
        }
    }

    return maxlen;
}

int main() {
    // char str[] = "abcabcbb";
    char str[] = "pwwkew";
    int length = lengthOfLongestSubstring(str);
    printf("Length of longest substring without repeating characters: %d\n", length);
    return 0;
}
