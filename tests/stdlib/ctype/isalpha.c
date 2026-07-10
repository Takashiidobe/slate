#include <ctype.h>
#include <stdio.h>
int main(void) { int volatile a='A', b=' '; printf("%d %d\n", isalpha(a)?1:0, isalpha(b)?1:0); return 0; }
