#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

static atomic_int counter = 0;

static void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < 100000; i++) {
        atomic_fetch_add(&counter, 1);
    }
    return 0;
}

int main(void) {
    pthread_t threads[4];
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], 0, worker, 0);
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], 0);
    }
    printf("%d\n", (int)counter);
    return 0;
}
