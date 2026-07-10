#include <errno.h>
#include <string.h>
#include <stdio.h>
int main(void) { errno = 0; printf("%d\n", errno); return 0; }
