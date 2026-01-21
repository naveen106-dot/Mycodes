#include <stdio.h>
#include <stdlib.h>

struct Node{
   
    int val;
    struct Node *next;

};

struct Node * createLinkedList(int num){
    struct Node *head=NULL,*temp = NULL,*node = NULL,*random = NULL;
    for( int i=1;i<=num;i++){
        node =(struct Node *) malloc(sizeof(struct Node));

        printf("enter the number:%d",i);
        scanf("%d",&node->val);
        node->next = NULL;

        if(head == NULL){
            head = node;
            temp = node;
        }else{
            temp->next = node;
            temp = node;  
        }
        if(i == 2){
            random = node;
        }
    }
    temp->next = random;

    return head;
}

void checkCircularlist(struct Node *head){
    struct Node *s = head,*f = head,*temp = NULL;
    while(temp != NULL){
        

    }
}



void printlinkedlist(struct Node *head,int num){
    struct Node *temp = head;
    
    for(int i=1;i<=num;i++){
         printf("|%d|%p|->",temp->val,temp->next);
        temp = temp->next;
    }
}
int main(){
    int num = 5;
    struct Node *head = createLinkedList(num);
    printlinkedlist(head, num);
    checkCircularlist(head);
}