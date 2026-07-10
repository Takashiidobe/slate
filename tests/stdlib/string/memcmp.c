#include <string.h>
#include <stdio.h>
int main(void) { const char *volatile a = "abc"; const char *volatile b = "abd"; printf("%d\n", memcmp((char*)a,(char*)b,3)<0); return 0; }
