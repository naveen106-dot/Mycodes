#include <stdio.h>


/*
Function declarations 
*/
void run_test(const char *test_name, int condition);

void push(int num);
int pop();
int peek();
int isEmpty();
/*
Global variables
*/

#define MAX 100 

int top =  -1;

int stack[MAX];



void test_stack() {
    top = -1;  // reset stack before testing

    // Test 1: Stack should be empty initially
    run_test("Initial stack empty", isEmpty() == 0);

    // Test 2: Push 10
    push(10);
    run_test("Push 10", peek() == 10);

    // Test 3: Push 20
    push(20);
    run_test("Push 20", peek() == 20);

    // Test 4: Pop should give 20
    int p1 = pop();
    run_test("Pop returns 20", p1 == 20);

    // Test 5: Pop should give 10
    int p2 = pop();
    run_test("Pop returns 10", p2 == 10);

    // Test 6: Pop on empty → underflow
    int p3 = pop();
    run_test("Pop underflow returns -1", p3 == -1);

    // Test 7: Peek on empty
    int p4 = peek();
    run_test("Peek underflow returns -1", p4 == -1);

    // Test 8: Fill full stack
    top = -1;
    for(int i=0;i<MAX;i++) push(i);
    run_test("Stack full, top is MAX-1", top == MAX - 1);

    // Test 9: Push on overflow
    int before = top;
    push(1000);
    run_test("Push overflow (top unchanged)", top == before);
}

void run_test(const char *test_name, int condition) {
    if (condition)
        printf("[PASS] %s\n", test_name);
    else
        printf("[FAIL] %s\n", test_name);
}


void push(int num){
    if(top == MAX -1){
        printf("Stack overflow");
    }
    stack[++top] = num;
}
int pop(){
    if(top == -1){
        printf("stack is underflow");
        return -1;
    }
    return stack[top--];
}

int peek(){
    if(top == -1){
        printf("in So values\n");
        return -1;
    }
    return stack[top];
}

int isEmpty(){
    if(top == -1){
        printf("stack is Empty");
        return 0;
    }
    return 1;
}


int main(){
    test_stack();
    return 0;
}