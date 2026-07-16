#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
    _Atomic(float) f = 1.5f;
    float old_f = atomic_fetch_add_explicit(&f, 2.25f, memory_order_relaxed);
    float now_f = atomic_load(&f);

    int values[4] = {10, 20, 30, 40};
    _Atomic(int *) p = values;
    int *old_p = atomic_fetch_add_explicit(&p, 2, memory_order_acq_rel);
    int *now_p = atomic_load(&p);
    int *old_x = atomic_exchange_explicit(&p, values + 1, memory_order_release);
    int *now_x = atomic_load(&p);

    printf("%.2f %.2f %td %td %d %td %td\n",
           old_f, now_f, old_p - values, now_p - values, *now_p, old_x - values, now_x - values);
    return 0;
}
