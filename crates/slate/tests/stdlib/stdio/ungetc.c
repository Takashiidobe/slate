#include <stdio.h>
int main(void) { int c = getchar(); ungetc(c, stdin); int d = getchar(); printf("%c%c\n", c, d); return 0; }
