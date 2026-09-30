#include <ulimit.h>

#ifndef UL_GETFSIZE
#error "ulimit.h:UL_GETFSIZE macro is missing from libc-shim"
#endif

#ifndef UL_SETFSIZE
#error "ulimit.h:UL_SETFSIZE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
