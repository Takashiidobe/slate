#include <stdio.h>

static int idiv(int a, int b) {
    return a / b;
}

int main(void) {
    printf("%d\n", idiv(-7, 2));
    printf("%d\n", idiv(7, -2));
    unsigned int u = 100u;
    printf("%u\n", u / 7u);
    return 0;
}
