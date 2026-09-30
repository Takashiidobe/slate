#include <sys/vlimit.h>

#ifndef INFINITY
#error "sys/vlimit.h:INFINITY macro is missing from libc-shim"
#endif

int main(void) { return 0; }
