#include <stdio.h>
#include <stdatomic.h>

struct Counters {
    _Atomic int value;
    volatile unsigned short small;
};
static _Atomic int shared = 7;
static int calls;
static int callback(int x) { return x + 5; }
static volatile int *once(volatile int *p) {
    ++calls;
    return p;
}
int main(void) {
    unsigned char a = 231;
    short b = -1234;
    int c = -765432;
    unsigned long long d = 0xfedcba9876543210ULL;
    int *p = &c;
    const int *cp = &c;
    int (*fn)(int) = callback;
    volatile _Bool flag = 1;
    volatile int v = 17;
    volatile unsigned short s = 65000;
    volatile unsigned long long wide = d;
    struct Counters counters = {3, 9};
    int av = __atomic_load_n(&a, __ATOMIC_RELAXED);
    int bv = __atomic_load_n(&b, __ATOMIC_CONSUME);
    int cv = __atomic_load_n(&c, __ATOMIC_ACQUIRE);
    unsigned long long dv = __atomic_load_n(&d, __ATOMIC_SEQ_CST);
    int *pv = __atomic_load_n(&p, __ATOMIC_ACQUIRE);
    const int *cpv = __atomic_load_n(&cp, __ATOMIC_RELAXED);
    __atomic_store_n(&fn, callback, __ATOMIC_RELAXED);
    int (*fnv)(int) = __atomic_load_n(&fn, __ATOMIC_ACQUIRE);
    if (*cpv != c || fnv(7) != 12 || !__atomic_load_n(&flag, __ATOMIC_RELAXED)) return 1;
    int vv = __atomic_load_n(once(&v), __ATOMIC_RELAXED);
    unsigned short sv = __atomic_load_n(&s, __ATOMIC_ACQUIRE);
    unsigned long long wv = __atomic_load_n(&wide, __ATOMIC_SEQ_CST);
    __atomic_load_n(&v, __ATOMIC_RELAXED);
    __atomic_store_n(&v, 29, __ATOMIC_SEQ_CST);
    atomic_store_explicit(&shared, 11, memory_order_release);
    atomic_store_explicit(&counters.value, 13, memory_order_relaxed);
    __atomic_store_n(&counters.small, 19, __ATOMIC_RELEASE);
    __atomic_thread_fence(__ATOMIC_RELAXED);
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    __atomic_thread_fence(__ATOMIC_RELEASE);
    __atomic_thread_fence(__ATOMIC_ACQ_REL);
    __atomic_thread_fence(__ATOMIC_SEQ_CST);
    __atomic_signal_fence(__ATOMIC_ACQUIRE);
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    printf("%d %d %d %llu %d %d %u %llu %d %d %d %u %d\n",
           av, bv, cv, dv, *pv, vv, sv, wv, calls, v,
           atomic_load_explicit(&shared, memory_order_acquire),
           counters.small, atomic_load_explicit(&counters.value, memory_order_seq_cst));
    return 0;
}
