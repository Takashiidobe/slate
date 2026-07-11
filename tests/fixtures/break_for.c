#include <stdio.h>

static int first_multiple(int n, int m) {
    for (int i = 1; i <= n; i++) {
        if (i % m == 0) {
            return i;
        }
        if (i > 100) {
            break;
        }
    }
    return -1;
}

int main(void) {
    printf("%d\n", first_multiple(50, 7));
    printf("%d\n", first_multiple(3, 7));
    return 0;
}
