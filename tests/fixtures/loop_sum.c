#include <stdio.h>

static int sum_to(int n) {
    int total = 0;
    for (int i = 1; i <= n; i++) {
        total += i;
    }
    return total;
}

int main(void) {
    printf("%d\n", sum_to(10));
    printf("%d\n", sum_to(1));
    printf("%d\n", sum_to(0));
    return 0;
}
