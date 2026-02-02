#include <stdio.h>
#include <stdlib.h>

struct Node{
    int value;
    struct Node *next;
};

struct Node* createLinkedList(int num){
    static int add = 100;
    struct Node *head = NULL,*temp = NULL,*node = NULL;
    for(int i=0;i<num;i++)
    {
        node = (struct Node *) malloc(sizeof(struct Node));
        node->value = add;
        add = add + 2;
        node->next = NULL;
        if(head == NULL){
            head = node;
            temp = node;
        }else{
            temp->next = node;
            temp = temp->next;
        }
    }
    return head;
}
void printLinkedList(struct Node *head)
{

    struct Node *temp = head;
    if(temp == NULL){
        printf("temp is Null \n");
    }
    while(temp != NULL){
        printf("%d ",temp->value);
        temp = temp->next;
    }
    printf("\n");
}

struct Node* insertAtBegining(struct Node *head){
    struct Node *temp = NULL;
    temp = (struct Node*) malloc(sizeof(struct Node));
    temp->value = 200;
    temp->next = head;
    return temp; 
}

struct Node* insertAtEnd(struct Node *head){
    struct Node *temp = head,*node = NULL;
    node = (struct Node*) malloc(sizeof(struct Node));
   
    node->value = 202;
    node->next = NULL;
    
    while(temp->next != NULL){
        temp = temp->next;
    }
    printf("\ntemp last:%d \n",temp->value);
    temp->next = node;

    return head; 
}

void Insertposition(struct Node *head, int pos)
{
    struct Node *temp = head,*new = NULL,*dummy = NULL;
    new = (struct Node *) malloc(sizeof(struct Node));
    new->value = 52;
    new->next = NULL;
    for(int i=1;i<pos;i++)
    {
        temp = temp->next;
    }

    dummy = temp->next;
    temp->next = new;
    new->next = dummy;
}
/****************Delete Operation in node******************** */
struct Node* deleteFromBeginning(struct Node *head)
{
    struct Node *temp = head->next;
    free(head);
    return temp;
}
struct Node * deleteFromEnd(struct Node *head)
{
    if(head == NULL)
    {
     return NULL;   
    }
    if(head->next == NULL)
    {
        free(head->next);
        return NULL;
    }
    struct Node *temp = head;
    while(temp->next->next != NULL)
    {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
}

void deleteNodeByValue(struct Node *head,int value)
{
    printf("\nfunction called: deleteNodeByValue\n");
    struct Node *curr = head,*dummy = NULL;
    struct Node *prev = NULL;
    
    while(curr != NULL && curr->value != value)
    {
      prev = curr;
      curr = curr->next;
    }
    printf("curr%d\n",curr->value);
    dummy = curr->next;
    free(prev->next);
    prev->next = dummy; 
}
void deleteNodePos(struct Node *head,int pos)
{
    struct Node *curr = head,*prev = NULL,*dummy = NULL;
    if(head == NULL)
        printf("head is null");
    
    for(int i=1;i<pos;i++){
        prev = curr;
        curr = curr->next;
    }
    dummy = curr->next;
    free(prev->next);
    prev->next = dummy;

}

struct Node* deleteEntireNodes(struct Node* head)
{
    struct Node *temp = head,*dummy = NULL;
    while(temp != NULL)
    {
        dummy = temp->next;
        free(temp);
        temp = dummy;
    }
   // printf("all nodes are delted \n");
   return head;

}
/***************print reverese linked list *****************/
void reverseListPrint(struct Node *head)
{
    
    int arr[100];
    int index = 0;
    while (head != NULL)
    {
        arr[index++] = head->value;
        head = head->next;
    }
    for(int i=index;i>0;i--){
        printf("%d ",arr[i]);
    }
}

/****************search the value***********************/
struct Node* searchIterative(struct Node *head,int value)
{
    struct Node *temp = head;
    if (head == NULL)
        return NULL;
    
    while(temp != NULL)
    {
        if(temp->value == value)
        {
            return temp;
        }
        temp = temp->next;
    }
    
    return NULL;
}
/**************find Middle Node ***********************/
struct Node* findMiddleNode(struct Node *head)
{
    struct Node *slow = head;
    struct Node *fast = head;
    while(fast->next != NULL)
    {
        printf("slow :%d - fast :%d\n",slow->value,fast->value);
        slow = slow->next;
        fast = fast->next->next;
        
    }
    return slow;
}
int main()
{
    int num = 10;
    int pos = 5;
    int val = 112;
    struct Node *head = NULL;
    head = createLinkedList(num);
    printLinkedList(head);
    head = insertAtBegining(head);
    printf("after added first node\n ");
    printLinkedList(head);
     head = insertAtEnd(head);
    printf("after added last node\n ");
    printLinkedList(head);
    Insertposition(head,pos);
    printf("inserting node at position\n");
    printLinkedList(head);
    head = deleteFromBeginning(head);
    printf("After deleted the first Node\n");
    printLinkedList(head);
    deleteFromEnd(head);
    printf("After deleted the last Node\n");
    printLinkedList(head);
    deleteNodeByValue(head,val);
    printf("After deleted by node by it's value\n");
    printLinkedList(head);
    deleteNodePos(head,pos);
    printf("After deleted by node by it's position\n");
    printLinkedList(head);
    //deleteEntireNodes(head);
    //reverseListPrint(head);
    struct Node *found = searchIterative(head,114);
    if(found->value == 104)
        printf("found value\n");
    
    findMiddleNode(head);
    
    
    
    


}