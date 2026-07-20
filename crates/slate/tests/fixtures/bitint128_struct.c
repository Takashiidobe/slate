#include <stdio.h>

struct W {
    int tag;
    _BitInt(128) s;
    unsigned _BitInt(128) u;
};

int main(void) {
    struct W w;
    w.tag = 1;
    w.s = -5;
    w.u = 12345678901234567890ULL;
    w.s += 10;
    w.u *= 2;

    printf("%zu\n", sizeof(struct W));
    printf("%lld\n", (long long)w.s);
    printf("%llu\n", (unsigned long long)w.u);
    return 0;
}
