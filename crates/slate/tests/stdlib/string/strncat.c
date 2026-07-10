#include <string.h>
#include <stdio.h>
int main(void) { char b[16] = "foo"; strncat(b, "barbaz", 3); puts(b); return 0; }
