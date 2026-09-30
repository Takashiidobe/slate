#include <sys/errno.h>

#ifndef ENOTSUP
#error "sys/errno.h:ENOTSUP macro is missing from libc-shim"
#endif

int main(void) { return 0; }
