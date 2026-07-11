#include <stdio.h>

static int sum_odds(int n) {
    int total = 0;
    for (int i = 0; i <= n; i++) {
        if (i % 2 == 0) {
            continue;
        }
        total += i;
    }
    return total;
}

int main(void) {
    printf("%d\n", sum_odds(10));
    printf("%d\n", sum_odds(1));
    printf("%d\n", sum_odds(0));
    return 0;
}
