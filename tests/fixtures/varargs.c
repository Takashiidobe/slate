#include <stdarg.h>
#include <stdio.h>

static int sum(int n, ...) {
    va_list ap;
    va_start(ap, n);
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += va_arg(ap, int);
    }
    va_end(ap);
    return total;
}

static int pick_second(int marker, ...) {
    va_list ap;
    va_start(ap, marker);
    int first = va_arg(ap, int);
    int second = va_arg(ap, int);
    va_end(ap);
    return marker + first * 10 + second;
}

int main(void) {
    printf("%d\n", sum(4, 10, 20, 30, 40));
    printf("%d\n", pick_second(5, 7, 9));
    return 0;
}
