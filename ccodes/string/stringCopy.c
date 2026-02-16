#include <stdio.h>

char *my_strcpy(char *dest, const char *src)
{
    char *temp = dest;

    // Copy characters until null terminator
    while (*src != '\0') {
          printf("src:%c - des:%c\n",*src,*dest);
        *dest = *src;
        dest++;
        src++;
      
    }

    // Copy the null terminator
    *dest = '\0';

    return temp;
}

int main()
{
    char src[] = "HELLO Naveen How are you";
    char dest[10];

    my_strcpy(dest, src);

    printf("Source      : %s\n", src);
    printf("Destination : %s\n", dest);

    return 0;
}
