#include <stdio.h>

static int countdown(int n) {
    int steps = 0;
    while (1) {
        if (n <= 0) {
            break;
        }
        n /= 2;
        steps++;
    }
    return steps;
}

int main(void) {
    printf("%d\n", countdown(100));
    printf("%d\n", countdown(0));
    return 0;
}
