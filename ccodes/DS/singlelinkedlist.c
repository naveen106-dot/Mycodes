#include <stdio.h>
#include <stdlib.h>

struct Node{
    int value;
    struct Node *next;
};

void printlist(struct Node *h){
    struct Node *t = h;
    struct Node *rev = NULL;
    while(t != NULL ){
        printf("%d ",t->value);
            t = t->next;
    }
}

struct Node* startList(int num){
    struct Node *head = NULL,*node = NULL,*temp = NULL;
    for (int i=0;i<num;i++){
        node = malloc(sizeof(struct Node));
        printf("Enter the number:\n");
        scanf("%d",&node->value);
        node->next = NULL;    
    if(head == NULL){
        head = node;
        temp = node;
    }else{
        temp->next = node;
        temp = node;
    }
}
return head;
}

struct Node* insertAtBeginning(struct Node *head, int value){

    struct Node *newNode = malloc(sizeof(struct Node));
    newNode->value = value;

    newNode->next = head;
    
    return newNode;
}


struct Node* removeDuplicates(struct Node *head){
    printf("Hello\n");
    int count = 0;
    struct Node *temp = malloc(sizeof(struct Node));
    temp = head;
    while(temp->next != NULL){
        count = count + 1 ;
        temp = temp->next;
    }
    temp = head;
    printf("count:%d\n",count);
        int i = 0;
    while(temp->next != NULL){
        while(i<count){
            int value = temp->value;
            temp = temp->next;
            if(value ==  temp->value){
                printf("Value:%d\n",value);
                flag = 1;
            }
            if (flag == 1){
                
            }
            i = i+1;
        }
    }

    return head;
}

int main(){
    struct Node *head = startList(5);
    // printlist(head);
    int value = 100;
    head  = insertAtBeginning(head,value);
    printlist(head);

    removeDuplicates(head);
}