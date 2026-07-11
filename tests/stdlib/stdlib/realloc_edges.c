#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int *p = realloc(NULL, 2 * sizeof(int));
    if (!p) {
        puts("alloc failed");
        return 1;
    }
    p[0] = 11;
    p[1] = 31;
    printf("%d\n", p[0] + p[1]);
    void *q = realloc(p, 0);
    printf("%d\n", q == NULL || q != p);
    return 0;
}
