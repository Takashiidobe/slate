#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *end = 0;
    unsigned long long v = strtoull("ffxyz", &end, 16);
    printf("%llu %c\n", v, *end);
    return 0;
}
