#include <string.h>
#include <stdio.h>
int main(void) { char b[8] = "abcdef"; memmove(b+1, b, 3); b[7]=0; puts(b); return 0; }
