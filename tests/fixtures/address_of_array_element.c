#include <stdio.h>

int main(void) {
    int values[4] = {2, 4, 6, 8};
    int *p = &values[1];
    int *q = &values[3];
    printf("%d %ld\n", *p + *q, q - p);
    return 0;
}
