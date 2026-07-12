#include <stdio.h>

int main(void) {
    unsigned char buf[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    unsigned char *hit = (unsigned char *)__builtin_memchr(buf, 40, 8);
    unsigned char *miss = (unsigned char *)__builtin_memchr(buf, 99, 8);
    printf("%ld %d\n", (long)(hit - buf), miss == 0);
    return 0;
}
