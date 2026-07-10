#include <stdio.h>

static int ior(int a, int b) {
    return a | b;
}

int main(void) {
    printf("%d\n", ior(12, 10));
    printf("%d\n", ior(-8, 3));
    unsigned int u = 0xF0u;
    printf("%u\n", u | 0x0Fu);
    return 0;
}
