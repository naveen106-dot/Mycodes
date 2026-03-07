#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// char* convert(char* s, int numRows) {

//     int len = strlen(s);
//     if(len <= 1 ){
//         printf("returned zero");
//         return 0;
//     }
//     printf("len:%d\n",len);
//     char arr[numRows];
//     for(int i=0;i<numRows;i++){
//     char *alloc = calloc(len,sizeof(char));
//     arr[i] = &alloc;
//     }
   
//     int i=0,num =0;
//     while(len >= 0){
//         if(num <= numRows)
//             arr[num] = s[i];
//                 if(num <= 3)
//                     num = 0;

//         len--;
//         i++;
//     }
//     for(int i=0;i<len;i++)
//         for(int j=0;j<numRows;j++)
//             printf("%c",arr[j]);
// }

void convert(char *s,int size){
    int len = 0;
    while(s[len] != '\0'){
        len++;
    }
    printf("len:%d size:%d ",len,size);
    int index =0;
    for(int i=0;i<size;i++){  
        for(int j=i;j<len;j+=size){
              printf("%c",s[j]);
        }
        
    }

}

int main(){
    char str[] = "PAYPALISHIRING";
    
    // output = "PAHNAPLSIIGYIR";
    convert(str,3);

}