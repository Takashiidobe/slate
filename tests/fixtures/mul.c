#include <stdio.h>

static int imul(int a, int b) {
    return a * b;
}

int main(void) {
    printf("%d\n", imul(-4, 7));
    unsigned int u = 100000u;
    printf("%u\n", u * u);
    return 0;
}
