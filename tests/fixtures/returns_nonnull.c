#include <stdio.h>

__attribute__((returns_nonnull)) int *get(int *a) {
    return a;
}

int main(void) {
    int x = 5;
    printf("%d\n", *get(&x));
    return 0;
}
