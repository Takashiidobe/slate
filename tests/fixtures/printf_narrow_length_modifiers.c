#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

int main(void) {
    short s = 300;
    unsigned short us = 60000;
    signed char c = -5;
    unsigned char uc = 200;
    intmax_t j = 123456789L;
    uintmax_t ju = 123456789UL;
    ptrdiff_t t = -7;
    printf("%hd %hu %hhd %hhu %jd %ju %td\n", s, us, c, uc, j, ju, t);

    int full_int = 300;
    int negative = -1;
    unsigned int wide = 70000;
    printf("%hhd %hhd %hhu %hd %hu\n", full_int, uc, negative, wide, wide);
    return 0;
}
