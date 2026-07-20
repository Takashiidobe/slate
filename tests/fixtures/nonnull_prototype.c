#include <stdio.h>

int use_it(int *a, int *b) __attribute__((nonnull(1, 2)));

int main(void) {
    int x = 6;
    int y = 2;
    printf("%d\n", use_it(&x, &y));
    return 0;
}

int use_it(int *a, int *b) {
    return *a / *b;
}
