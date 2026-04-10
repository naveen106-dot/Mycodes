#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// void pattern(){
//     int n = 3;
//     int size = n*2;
//     printf("size:%d\n",size);
//     for(int i=0;i<size;i++){
//         for(int j=0;j<size;j++){
//             printf(" %d ",n-i-j);
//         }
//         printf("\n");
//     }
// }
int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    
    int n;
    printf("Enter the size of an array:");
    scanf("%d",&n);
    int arr[n];
    printf("print the array values:%d\n",n);
    for(int i=0;i<n;i++){
    
        scanf("%d",&arr[i]);
    
    }
    for(int j=0;j<n;j++){
        printf("%d ",arr[j]);
    }

    int *total = (int *)malloc(n*sizeof(int));
    static int cal = 0;
    
    for(int i=0;i<n;i++){
        cal += arr[i];
    }
    printf("cal :%d\n",cal);
    *total = cal;
    printf("total :%d\n",*total);
    
    free(total);
    // pattern();
    return 0;
}
