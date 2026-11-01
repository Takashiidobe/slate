#include <stdio.h>

#define SHOW(a) { printf("%d\n", (int)(a)); }
#define ONLY_HERE(a) { printf("helper %d\n", (int)(a)); }
#define WIDEN(a) { printf("%ld\n", (long)(a)); }

int helper(int a) {
    SHOW(a);
    ONLY_HERE(a);
    ONLY_HERE(a + 1);
    WIDEN(a);
    return a * 2;
}
