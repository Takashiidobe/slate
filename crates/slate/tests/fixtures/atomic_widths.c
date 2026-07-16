#include <stdatomic.h>
#include <stdio.h>

int main(void) {
    atomic_uchar u8 = 250;
    atomic_schar i8 = -5;
    atomic_uint u32 = 1000u;
    atomic_llong i64 = -10000000000LL;

    unsigned char old_u8 = atomic_fetch_add_explicit(&u8, 3, memory_order_relaxed);
    signed char old_i8 = atomic_fetch_sub_explicit(&i8, 7, memory_order_acq_rel);
    unsigned int old_u32 = atomic_fetch_xor_explicit(&u32, 0x00FFu, memory_order_release);
    long long old_i64 = atomic_exchange_explicit(&i64, 1234567890123LL, memory_order_acquire);

    printf("%u %d %u %lld %u %d %u %lld\n",
           old_u8, old_i8, old_u32, old_i64,
           (unsigned char)u8, (signed char)i8, (unsigned int)u32, (long long)i64);
    return 0;
}
