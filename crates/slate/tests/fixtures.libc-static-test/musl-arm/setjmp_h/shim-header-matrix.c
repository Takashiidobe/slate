#include <setjmp.h>

#ifndef setjmp
#error "setjmp.h:setjmp macro is missing from libc-shim"
#endif

int main(void) { return 0; }
