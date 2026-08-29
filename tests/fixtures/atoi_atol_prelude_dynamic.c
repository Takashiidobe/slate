#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char a[32];
    char b[32];
    char c[32];
    strcpy(a, "  -42abc");
    strcpy(b, "+1000000000000zzz");
    strcpy(c, "9000000000000");
    printf("%d\n", atoi(a));
    printf("%ld\n", atol(b));
    printf("%lld\n", atoll(c));
    return 0;
}
