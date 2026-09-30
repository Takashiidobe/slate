#include <ucontext.h>

#ifndef NGREG
#error "ucontext.h:NGREG macro is missing from libc-shim"
#endif

int main(void) { return 0; }
