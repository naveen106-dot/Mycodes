#include <stdio.h>

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    for(int i = 0; i < 31; i++) {
        if (num & (1 << i)) {
            num = num ^ (1 << i);   // Turn off the bit
            break;
        }
    }

    printf("Number after turning off rightmost set bit = %d\n", num);

    return 0;
}
