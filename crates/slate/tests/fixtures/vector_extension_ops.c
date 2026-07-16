#include <stdio.h>

typedef int v4si __attribute__((vector_size(16)));

int main(void) {
    v4si a = {1, 2, 3, 4};
    v4si b = {5, 6, 7, 8};
    v4si c = a + b;
    c[1] = 20;
    v4si d = __builtin_shufflevector(c, c, 3, 2, 1, 0);
    printf("%d %d %d\n", c[0], c[1], d[0]);
    return 0;
}
