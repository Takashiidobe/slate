#include <stdio.h>

static int calls;

static unsigned long long next(void) {
    calls++;
    return 0x100000000ULL;
}

int main(void) {
    volatile unsigned long long values[] = {1, 2, 3, 0x80000000ULL, 0x100000000ULL, 0x8000000000000000ULL, ~0ULL};
    for (unsigned i = 0; i < sizeof(values) / sizeof(values[0]); i++)
        printf("%d ", __builtin_clzll(values[i]));
    volatile unsigned int small = 1;
    volatile unsigned long wide = 1;
    printf("%d %d %d ", __builtin_clz(small), __builtin_clzl(wide), __builtin_clzll(next()));
    printf("%d\n", calls);
    return 0;
}
