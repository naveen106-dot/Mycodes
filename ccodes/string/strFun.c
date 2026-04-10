#include <stdio.h>
#include <string.h>
#include <stdint.h>


int main(){
    char src[20] = "hello Naveen";
    char des[20];

    fgets(des,20,stdin);
    //strcpy(des,src);  // copy source to destination string 
    printf("fgets after :%s\n",des);

   // strncpy(des,src,4);        // copy number bytes to copy from source to destination 

    printf("after :%s\n",des);

    char s1[20] = "hello";
    char s2[20] = "hello ";
    //strcat(s1,s2);
    //printf("concat :%s\n",s1); // concating two string 

    //strncat(s1,s2,5);
    //printf("concating N char:%s\n",s1);


    int temp = strcmp(s1,s2); //comparing the two string using lexigrphycally
    printf("temp: %d\n",temp);

    // char *str = strchr(src ,'n');
    // if(str != NULL){
    //     printf("character occured :%d",*str-src);
    // }

    char input[20];
    int n = 10;
    sprintf(input,"the value of %d",n);
    printf("input :%s\n",input);
    int length = strlen(input);
    printf("length:%d\n",length);




}