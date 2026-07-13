#include <stdio.h>
#include <string.h>

int main(void) {
    char alpha[] = "abc";
    char beta[] = "abd";
    unsigned char bytes_a[] = "\xff\x01";
    unsigned char bytes_b[] = "\xff\x02";

    printf("%zu %d %d %d %d\n",
           strlen(alpha),
           strcmp(alpha, alpha) == 0,
           strcmp(alpha, beta) < 0,
           strncmp(alpha, beta, 2) == 0,
           memcmp(bytes_a, bytes_b, 1) == 0);
    return 0;
}
