#include <stdio.h>

int main(void) {
    double x = 1234.5678;
    printf("%g %.3g %-10.3g\n", x, x, x);
    printf("%G %.3G\n", x, x);
    return 0;
}
