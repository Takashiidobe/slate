#include <string.h>
#include <stdio.h>
int main(void) { const char *volatile v = "hello"; const char *s=(char*)v; printf("%ld\n", strrchr(s,'l')-s); return 0; }
