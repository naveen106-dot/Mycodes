#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void* thread1(void* arg) {
    pthread_mutex_lock(&lock1);
    printf("Thread-1 acquired lock1\n");

    sleep(1);  // simulate work

    printf("Thread-1 waiting for lock2\n");
    pthread_mutex_lock(&lock2);   // DEADLOCK here

    printf("Thread-1 acquired lock2\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* thread2(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Thread-2 acquired lock2\n");

    sleep(1);  // simulate work

    printf("Thread-2 waiting for lock1\n");
    pthread_mutex_lock(&lock1);   // DEADLOCK here

    printf("Thread-2 acquired lock1\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Program finished\n");
    return 0;
}
