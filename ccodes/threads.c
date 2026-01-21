#include <stdio.h>
#include <threads.h>

mtx_t lock;
int num = 'a';

int thread1(void *arg) {
    for (int i = 0; i < 13; i++) {
        mtx_lock(&lock);
        printf("Thread1: %c\n", num);
        num++;
        mtx_unlock(&lock);
        thrd_yield();   // give chance to other thread
    }
    return 0;
}

int thread2(void *arg) {
    for (int i = 0; i < 12; i++) {
        mtx_lock(&lock);
        printf("Thread2: %c\n", num);
        num++;
        mtx_unlock(&lock);
        thrd_yield();
    }
    return 0;
}

int main() {
    thrd_t t1, t2;

    mtx_init(&lock, mtx_plain);

    thrd_create(&t1, thread1, NULL);
    thrd_create(&t2, thread2, NULL);

    thrd_join(t1, NULL);
    thrd_join(t2, NULL);

    mtx_destroy(&lock);
    return 0;
}
