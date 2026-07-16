#include <stdio.h>

static int likely_nonzero(int x) {
    if (__builtin_expect(x != 0, 1)) {
        __builtin_assume(x != 0);
        return x + 10;
    }
    return -1;
}

static int guarded_trap(int x) {
    if (x < 0) {
        __builtin_trap();
    }
    return x;
}

static int guarded_unreachable(int x) {
    if (x < 0) {
        __builtin_unreachable();
    }
    return x * 2;
}

int main(void) {
    volatile int input = 5;
    int a = likely_nonzero(input);
    int b = guarded_trap(input);
    int c = guarded_unreachable(input);
    printf("%d %d %d\n", a, b, c);
    return 0;
}
