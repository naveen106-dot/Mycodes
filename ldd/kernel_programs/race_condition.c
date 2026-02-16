#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int count = 0;

void* increment(void* arg) {
    int thread_no = *(int*)arg;

    for (int i = 0; i < 10000; i++) {
        //int temp = count;   // read
        //temp++;             // modify
        count++;       // write // last the update at certain point 

        printf("Thread-%d incremented count to %d\n", thread_no, count);
        usleep(100);        // force context switch
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_create(&t1, NULL, increment, &id1);
    pthread_create(&t2, NULL, increment, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final count: %d\n", count);
    return 0;
}
