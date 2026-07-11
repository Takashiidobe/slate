#include <ctype.h>
#include <stdio.h>
int main(void) { int volatile lo=0, del=127, hi=255; printf("%d %d %d\n", iscntrl(lo)?1:0, isprint(del)?1:0, isprint(hi)?1:0); return 0; }
