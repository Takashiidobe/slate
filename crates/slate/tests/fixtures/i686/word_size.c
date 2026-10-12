#include <stdio.h>
int main(void) { long l = 5; printf("%zu %zu %ld\n", sizeof(void *), sizeof(long), l); return 0; }
