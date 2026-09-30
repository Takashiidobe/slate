#include <sys/dir.h>

#ifndef direct
#error "sys/dir.h:direct macro is missing from libc-shim"
#endif

int main(void) { return 0; }
