#include <ctype.h>
#include <stdio.h>
int main(void) { int volatile a='a', b='F', c='g'; printf("%d %d %d\n", isxdigit(a)?1:0, isxdigit(b)?1:0, isxdigit(c)?1:0); return 0; }
