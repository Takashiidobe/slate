#include <stdio.h>
int main(void) { char b[16]; fgets(b, sizeof b, stdin); fputs(b, stdout); return 0; }
