#include <stdio.h>

static int ishr(int a, int b) {
    return a >> b;
}

int main(void) {
    printf("%d\n", ishr(256, 3));
    printf("%d\n", ishr(-256, 3));
    unsigned int u = 0x80000000u;
    printf("%u\n", u >> 4);
    return 0;
}
