#include <stdio.h>
#include <sys/syscall.h>

int main() { printf("%d\n", SYS_preadv); }

// SLATE-TARGETS: x86_64-linux-gnu,aarch64-linux-gnu
