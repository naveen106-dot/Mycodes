#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* convert(char* s, int numRows) {
    
    int len = strlen(s);
    if(len > 1 && numRows > 1){
        return 0;
    }
    printf("len:%d",len);

    int **rows = (char**)malloc(numRows * sizeof(char *));
    for(int i=0;i<numRows;i++){
        rows[i] = calloc(len+1,sizeof(char));
    }

    for(int i=0;i<numRows;i++)
    {
        for(int j=0;j<len;j++)
        {
           
        }
    }
}

int main(){
    char str[] = "PAYPALISHIRING";
    // output = "PAHNAPLSIIGYIR";
    convert(str,3);

}