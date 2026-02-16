#include <stdio.h>
#include <stddef.h>

void *my_memmove(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    // If same address or nothing to copy
    if (d == s || n == 0)
        return dest;

    // Forward copy (no overlap or safe overlap)
    if (d < s) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    }
    // Backward copy (overlapping regions)
    else {
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    
    return dest;
}

int main()
{
    char str1[20] = "HELLO";
    char str2[20];

    /* Case 1: Normal copy (no overlap) */
    my_memmove(str2, str1, 6);   // including '\0'
    printf("Normal copy  : %s\n", str2);

    /* Case 2: Overlapping memory copy */
    my_memmove(str1 + 2, str1, 3);
    printf("Overlap copy : %s\n", str1);

    return 0;
}
