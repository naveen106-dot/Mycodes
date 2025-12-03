#include <stdio.h>
#define SIZE 10

int queue[SIZE];
int front = -1,rear = -1;

void run_test(const char *name, int condition);
void enqueue(int num);
int dequeue();
void display();

void run_test(const char *name, int condition) {
    if (condition)
        printf("[PASS] %s\n", name);
    else
        printf("[FAIL] %s\n", name);
}
void test_queue() {
    front = rear = -1;  // Reset

    // Test 1: Queue initially empty
    run_test("Empty queue on start", dequeue() == -1);

    // Test 2: Enqueue 10
    enqueue(10);
    run_test("Front = 0 after first enqueue", front == 0);
    run_test("Rear = 0 after first enqueue", rear == 0);

    // Test 3: Enqueue 20
    enqueue(20);
    run_test("Rear increment after enqueue", rear == 1);

    // Test 4: Dequeue → returns 10
    run_test("Dequeue returns 10", dequeue() == 10);

    // Test 5: Dequeue → returns 20
    run_test("Dequeue returns 20", dequeue() == 20);

    // Test 6: Underflow check
    run_test("Dequeue on empty returns -1", dequeue() == -1);

    // Test 7: Fill queue completely
    front = rear = -1;
    for (int i = 0; i < SIZE; i++) enqueue(i);

    run_test("Queue full when rear == SIZE-1", rear == SIZE - 1);

    // Test 8: Overflow attempt
    int before = rear;
    enqueue(999);
    run_test("Overflow doesn't change rear", rear == before);
}


void enqueue(int num) {
    if (rear == SIZE - 1) {
        printf("Queue Overflow\n");
        return;
    }
    if (front == -1) front = 0;

    queue[++rear] = num;
    printf("Enqueued: %d\n", num);
}

int dequeue() {
    if (front == -1 || front > rear) {
        printf("Queue Underflow\n");
        return -1;
    }

    int removed = queue[front++];
    printf("Dequeued: %d\n", removed);

    return removed;
}
void display() {
    if (front == -1 || front > rear) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue: ");
    for (int i = front; i <= rear; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}

int main() {
//     enqueue(10);
//     enqueue(20);
//     enqueue(30);
//    //display();
//     dequeue();
//     dequeue();
//     enqueue(40);
//     //display();

    test_queue();
}
