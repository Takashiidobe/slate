#include <stdio.h>
#include <sys/syscall.h>

int main() { printf("%d\n", SYS_preadv); }
