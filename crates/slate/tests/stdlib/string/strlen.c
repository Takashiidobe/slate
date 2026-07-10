#include <string.h>
#include <stdio.h>
int main(void) { const char *volatile s = "hello"; printf("%zu\n", strlen((char*)s)); return 0; }
