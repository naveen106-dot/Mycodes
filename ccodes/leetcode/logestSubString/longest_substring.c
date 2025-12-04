#include <stdio.h>
#include <string.h>
#include "longest_substring.h"

static char charArr[200];

int lengthOfLongestSubstring(char* s) {
    int n = strlen(s);
    int maxlen = 0;

    for (int i = 0; i < n; i++) {
        int index = 0;

        // Add first character
        charArr[index++] = s[i];

        for (int j = i + 1; j < n; j++) {
            int repeat = 0;

            // check if s[j] appears in current substring
            for (int k = 0; k < index; k++) {
                if (charArr[k] == s[j]) {
                    repeat = 1;
                    break;
                }
            }

            if (!repeat)
                charArr[index++] = s[j];
            else
                break;
        }

        // update max length
        if (index > maxlen)
            maxlen = index;

        // clear array
        memset(charArr, 0, index);
    }

    return maxlen;
}
