#include "test.h"
#include <threads.h>

static int counter = 0;
static mtx_t mutex;

int thread_func(void *arg) {
    int *val = (int *)arg;
    mtx_lock(&mutex);
    counter += *val;
    mtx_unlock(&mutex);
    return thrd_success;
}

int main() {
    thrd_t t1, t2;
    int arg1 = 10, arg2 = 20;

    ASSERT(thrd_success, mtx_init(&mutex, mtx_plain));

    ASSERT(thrd_success, thrd_create(&t1, thread_func, &arg1));
    ASSERT(thrd_success, thrd_create(&t2, thread_func, &arg2));

    int res1, res2;
    ASSERT(thrd_success, thrd_join(t1, &res1));
    ASSERT(thrd_success, thrd_join(t2, &res2));

    ASSERT(thrd_success, res1);
    ASSERT(thrd_success, res2);
    ASSERT(30, counter);

    mtx_destroy(&mutex);

    printf("OK\n");
    return 0;
}
