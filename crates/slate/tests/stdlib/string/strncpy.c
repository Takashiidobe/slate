#include <string.h>
#include <stdio.h>
int main(void) { char b[8] = {0}; strncpy(b, "abcdef", 3); puts(b); return 0; }
