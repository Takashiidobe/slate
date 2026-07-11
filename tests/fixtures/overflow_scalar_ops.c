#include <stdio.h>

int main(void) {
    int ri = 0;
    unsigned int ru = 0;
    long long rll = 0;

    int add_i = __builtin_add_overflow(2147483647, 1, &ri);
    printf("%d %d\n", add_i, ri);

    int sub_i = __builtin_sub_overflow((-2147483647 - 1), 1, &ri);
    printf("%d %d\n", sub_i, ri);

    int mul_i = __builtin_mul_overflow(1073741824, 2, &ri);
    printf("%d %d\n", mul_i, ri);

    int add_u = __builtin_add_overflow(4294967295u, 1u, &ru);
    printf("%d %u\n", add_u, ru);

    int mul_ll = __builtin_mul_overflow(3037000500LL, 3037000500LL, &rll);
    printf("%d %lld\n", mul_ll, rll);

    return 0;
}
