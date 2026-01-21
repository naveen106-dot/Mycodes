#include <stdio.h>
#include <string.h>

int expand(char *s, int left, int right) {
    int n = strlen(s);
    while (left >= 0 && right < n && s[left] == s[right]) {
        left--;
        right++;
    }
    return right - left - 1;  // palindrome length
}

char* longestPalindrome(char* s) {
    int start = 0, end = 0;
    int n = strlen(s);

    for (int i = 0; i < n; i++) {
        int len1 = expand(s, i, i);       // odd length palindrome
        int len2 = expand(s, i, i + 1);   // even length palindrome
        int len = (len1 > len2) ? len1 : len2;

        if (len > end - start + 1) {
            start = i - (len - 1) / 2;
            end = i + len / 2;
        }
    }

    //static char result[500];
    char *result = malloc((end - start + 2) * sizeof(char));
    int idx = 0;
    for (int i = start; i <= end; i++) {
        result[idx++] = s[i];
    }
    result[idx] = '\0';

    return result;
}

int main() {
    printf("%s\n", longestPalindrome("babad")); // bab OR aba
    printf("%s\n", longestPalindrome("cbbd"));  // bb
}
