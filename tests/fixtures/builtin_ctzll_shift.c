#include <stdio.h>
#include <stddef.h>

size_t lowest_bit(unsigned long long x) {
    return (size_t)1 << (size_t)__builtin_ctzll(x);
}

int main(void) {
    unsigned long long x = 0;
    scanf("%llu", &x);
    size_t n = lowest_bit(x);
    printf("%zu\n", n);
    return 0;
}
