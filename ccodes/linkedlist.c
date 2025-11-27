#include <stdio.h>
#include <stdlib.h>
struct node{
    int value;
    struct node *nextNode;
};

int main(){
    struct node *head1 = NULL,*next1 = NULL,*temp1 = NULL;
      struct node *head2 = NULL,*next2 = NULL,*temp2 = NULL;

    for(int i=0;i<3;i++){
        next1 = (struct node *)malloc(sizeof(struct node));
        printf("Enter the %d value",i);
        scanf("%d",&next1->value);
        next1->nextNode = NULL;
        if(head1 == NULL){
            head1 = next1;
            temp1 = next1;

        }else{
            
            temp1->nextNode= next1; 
            temp1 = next1;
        }
    }
    printf("\n");
    temp1 = head1;
    for (int i=0;i<3;i++){
        printf("%d \t",temp1->value);
        temp1 = temp1->nextNode;
    }


    for(int j=0;j<3;j++){
     next2=  (struct node *)malloc(sizeof(struct node));
     printf("Enter thr secode node %d",j+1);
     scanf("%d",&next2->value);
     next2->nextNode = NULL;
     if(head2 == NULL){
        head2 = next2;
        temp2 = head2;
     }else{
        temp2->nextNode = next2;
        temp2 = next2;
     }
    }

    printf("\n");
    temp2 = head2;
    for(int i=0;i<3;i++){
        printf("%d\t",temp2->value);
        temp2 = temp2->nextNode;
    }
    temp1 = head1;
    temp2 = head2;
    struct node *head3 = NULL,*temp3 = NULL,*next3 = NULL;

      int count=0,mod = 0,add1, add,carry=0;
    for(int i=0;i<3;i++){
        next3 = (struct node *)malloc(sizeof(struct node));
        int sum = temp1->value + temp2->value+carry;;
        next3->value = sum %10;
        carry = sum / 10;


        // while(add != 0)
        // {
        // add1 = add1/10;
        // count++;
        // }
        // printf("Count:%d",count);
        // if(count==2)
        // {
        // next3->value = add % 10;
        // mod = add / 10;
        // printf("rem:%d",mod);
        // }else
        // {
        // next3->value = add+mod;
        // }
        // printf("next->value:%d",next3->value);
        if(head3 == NULL)
        {
        head3 = next3;
        temp3 = head3;
        }else
        {
        temp3->nextNode = next3;
        temp3 = next3;
        }
        temp1 = temp1->nextNode;
        temp2 = temp2->nextNode;
    }

    printf("After Adding\n");
    printf("\n");
    temp3 = head3;
    for(int i=0;i<3;i++){
        printf("%d",temp3->value);
        temp3 = temp3->nextNode;
    }
return 0;
}

