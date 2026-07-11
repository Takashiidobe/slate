#include <string.h>
#include <stdio.h>
int main(void) { const char *volatile s = "abcdef"; char *p = memchr((char*)s, 'd', 6); printf("%ld\n", p - (char*)s); return 0; }
