#include <ctype.h>
#include <stdio.h>
int main(void) { int volatile c='a'; printf("%c\n", toupper(c)); return 0; }
