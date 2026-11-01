#include <stdio.h>

#define LOG(x) { fprintf x; }
#define ONLY_HERE(a) { printf("%d\n", (a) * 3); }
#define DIFFERS(a) { printf("helper %d\n", a); }

int helper(int value) {
    LOG((stdout, "helper %d\n", value));
    ONLY_HERE(value);
    DIFFERS(value);
    return value + 1;
}
