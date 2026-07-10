#include <string.h>
#include <stdio.h>
int main(void) { char b[6]; memset(b,'x',5); b[5]=0; puts(b); return 0; }
