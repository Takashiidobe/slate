#include <stdio.h>

static int sum_while(int n) {
    int total = 0;
    int i = 1;
    while (i <= n) {
        total += i;
        i++;
    }
    return total;
}

int main(void) {
    printf("%d\n", sum_while(5));
    printf("%d\n", sum_while(1));
    printf("%d\n", sum_while(0));
    return 0;
}
