#include <stdio.h>
#include <sys/syscall.h>

int main() { printf("%d\n", SYS_preadv); }

// SLATE-TARGETS: aarch64-linux-gnu,x86_64-linux-gnu,i686-linux-gnu,arm-linux-gnueabihf,arm-linux-musleabihf,arm-linux-musleabihf
