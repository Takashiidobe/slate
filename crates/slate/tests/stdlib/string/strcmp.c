#include <string.h>
#include <stdio.h>
int main(void) { const char *volatile a = "a"; const char *volatile b = "b"; printf("%d %d\n", strcmp((char*)a,(char*)a)==0, strcmp((char*)a,(char*)b)<0); return 0; }
