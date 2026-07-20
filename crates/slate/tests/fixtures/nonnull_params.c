#include <stdio.h>

__attribute__((nonnull(1, 2))) int add(int *a, int *b) {
    return *a + *b;
}

int main(void) {
    int x = 3;
    int y = 4;
    printf("%d\n", add(&x, &y));
    return 0;
}
