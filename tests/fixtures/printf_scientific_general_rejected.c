#include <stdio.h>

int main(void) {
    double x = 1234.5678;
    printf("%e %.2e %10.2e %+e\n", x, x, x, x);
    printf("%E %.2E\n", x, x);
    printf("%g %.3g %-10.3g\n", x, x, x);
    printf("%G %.3G\n", x, x);
    return 0;
}
