#include <stdio.h>

static int imod(int a, int b) {
    return a % b;
}

int main(void) {
    printf("%d\n", imod(-7, 3));
    printf("%d\n", imod(7, -3));
    unsigned int u = 100u;
    printf("%u\n", u % 7u);
    return 0;
}
