#include <stdio.h>
#include <string.h>

/*count the string length*/
int countStringLength(char str[]){
    printf("countStringLength called \n");
    int i=0,count = 0;
    while(str[i++] != '\0'){
        count++;
    }
    printf("count :%d\n",count);
    return count;
}

void reverse_str(char *str){
    printf("called reverse str\n");
    int count =  countStringLength(str);
    printf("count:%d\n",count);
    int i=0,j=count-1;
    while(i<j){
        char ch = str[i];
        str[i] = str[j];
        str[j] = ch; 
        i++;
        j--;
    }
    printf("string :%s\n",str);
}



/*
string = programming hellow
case :1 first count string once space came space to intial position 
case :2 then chnage the intial postion to 
*/
char * reverse_paragraph(char str[])
{
   // printf("\n reverse_paragraph \n");
   char arr[50] = {"\0"};
   int pos =0;
    int left = 0,right = 0,i=0;
    int count = countStringLength(str);
    while(i<=count){
       printf("i:%d count:%d\n",i,count);
        if(str[i] == ' ' || str[i] == '\0'){
            printf("i:%d\n",i);
            right = --i;
            while (left <= right)
            {
            //printf("%c ",str[right--]);   
            arr[pos++] = str[right--];
            }
            printf("\n");
            arr[pos++] = ' ';
            i++;
            left = i;
        }
        i++;
    }
    printf("str:%s\n",arr);
    return NULL;
}

int main(){

    char input[] = "programming hellow";
    reverse_str(input);
    char str[100];
    /***********using memmove copy of data *************************/
    memmove(str,"hello naveen how are you",25);
    int i =0;
    while(str[i] != '\0'){
        printf("%c",str[i++]);
    }

    printf("\n\n");
    /***********while store string it will allow space but while you use scanf("%s") i will not read after space  *************************/
    char string[] = "hello naveen how are you";
    reverse_paragraph(string);
    //printf("%s\n",string);


    /************correct way to write a string after space also******************/
    // char input1[50];
    // fgets(input1, sizeof(input1), stdin);
    // printf("%s", input1);
}
