#include <stdio.h>

int main(void) {
    unsigned int value = 48879u;
    unsigned int mask = 255u;
    unsigned long wide = 4095ul;
    unsigned int zero = 0u;
    printf("%#x %#X %#o\n", value, mask, mask);
    printf("%#08x|%-#10X|%#12lo\n", mask, mask, wide);
    printf("%#x %#X %#o %#08x\n", zero, zero, zero, zero);
    return 0;
}
