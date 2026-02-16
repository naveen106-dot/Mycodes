#include <stdio.h>
#define SIZE 5

int queue[SIZE];
int front = -1, rear = -1;

void enqueue(int value)
{
    if ((rear + 1) % SIZE == front)
    {
        printf("Queue is Full\n");
        return;
    }

    if (front == -1)  // First element
        front = 0;

    rear = (rear + 1) % SIZE;
    queue[rear] = value;

    printf("Inserted %d\n", value);
}

void dequeue()
{
    if (front == -1)
    {
        printf("Queue is Empty\n");
        return;
    }

    printf("Deleted %d\n", queue[front]);

    if (front == rear)   // Only one element
    {
        front = rear = -1;
    }
    else
    {
        front = (front + 1) % SIZE;
    }
}

void display()
{
    if (front == -1)
    {
        printf("Queue is Empty\n");
        return;
    }

    int i = front;
    while (1)
    {
        printf("%d ", queue[i]);
        if (i == rear)
            break;
        i = (i + 1) % SIZE;
    }
    printf("\n");
}

int main()
{
    enqueue(10);
    enqueue(20);
    enqueue(30);
    display();

    dequeue();
    display();

    enqueue(40);
    enqueue(50);
    enqueue(60);
    display();

    return 0;
}
