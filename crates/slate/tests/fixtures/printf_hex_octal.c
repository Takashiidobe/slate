#include <stdio.h>

int main(void) {
    unsigned int value = 48879u;
    unsigned int mask = 255u;
    unsigned long wide = 511ul;
    printf("%x %X %o\n", value, mask, wide);
    printf("%08x|%-4X|%5o\n", mask, mask, mask);
    return 0;
}
