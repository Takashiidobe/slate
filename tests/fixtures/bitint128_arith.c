#include <stdio.h>

static _BitInt(128) add128(_BitInt(128) a, _BitInt(128) b) {
    return a + b;
}

static unsigned _BitInt(128) mul128(unsigned _BitInt(128) a, unsigned _BitInt(128) b) {
    return a * b;
}

int main(void) {
    _BitInt(128) x = -12345;
    _BitInt(128) y = 100;
    _BitInt(128) sum = add128(x, y);
    printf("%lld\n", (long long)sum);

    unsigned _BitInt(128) p = 1000000000000ULL;
    unsigned _BitInt(128) q = 3;
    unsigned _BitInt(128) prod = mul128(p, q);
    printf("%llu\n", (unsigned long long)prod);
    return 0;
}
