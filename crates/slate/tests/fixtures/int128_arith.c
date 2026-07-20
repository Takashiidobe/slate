#include <stdio.h>

static __int128 add128(__int128 a, __int128 b) {
    return a + b;
}

static unsigned __int128 mul128(unsigned __int128 a, unsigned __int128 b) {
    return a * b;
}

static void print128(unsigned __int128 v) {
    unsigned long long hi = (unsigned long long)(v >> 64);
    unsigned long long lo = (unsigned long long)v;
    printf("%llu:%llu\n", hi, lo);
}

int main(void) {
    __int128 a = (__int128)9000000000000000000LL;
    __int128 b = (__int128)9000000000000000000LL;
    __int128 sum = add128(a, b);
    print128((unsigned __int128)sum);

    unsigned __int128 x = (unsigned __int128)1000000000000ULL;
    unsigned __int128 y = (unsigned __int128)1000000000000ULL;
    unsigned __int128 prod = mul128(x, y);
    print128(prod);

    int cmp = (sum > 0) ? 1 : 0;
    printf("%d\n", cmp);

    return 0;
}
