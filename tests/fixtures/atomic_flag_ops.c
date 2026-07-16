#include <stdatomic.h>
#include <stdio.h>

int main(void) {
    atomic_flag flag = ATOMIC_FLAG_INIT;

    int first = atomic_flag_test_and_set(&flag);
    atomic_flag_clear_explicit(&flag, memory_order_release);
    int second = atomic_flag_test_and_set_explicit(&flag, memory_order_acquire);
    int third = atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed);
    atomic_flag_clear(&flag);
    int fourth = atomic_flag_test_and_set(&flag);

    printf("%d %d %d %d\n", first, second, third, fourth);
    return 0;
}
