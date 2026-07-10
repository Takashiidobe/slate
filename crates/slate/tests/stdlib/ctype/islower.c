#include <ctype.h>
#include <stdio.h>
int main(void) { int volatile a='A', b=' '; printf("%d %d\n", islower(a)?1:0, islower(b)?1:0); return 0; }
