#include <threads.h>

#ifndef ONCE_FLAG_INIT
#error "threads.h:ONCE_FLAG_INIT macro is missing from libc-shim"
#endif

#ifndef TSS_DTOR_ITERATIONS
#error "threads.h:TSS_DTOR_ITERATIONS macro is missing from libc-shim"
#endif

#ifndef thread_local
#error "threads.h:thread_local macro is missing from libc-shim"
#endif

int main(void) { return 0; }
