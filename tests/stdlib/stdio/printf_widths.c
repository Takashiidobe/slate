#include <stdio.h>

int main(void) {
    int volatile n = 42;
    int m = -42;
    char s[] = "xy";
    printf("%05d|%-4s|%+d|%#x\n", n, s, m, n);
    return 0;
}
