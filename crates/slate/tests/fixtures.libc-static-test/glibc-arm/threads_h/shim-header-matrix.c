#include <threads.h>

typedef unsigned int slate_oracle_typedef_tss_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_tss_t, tss_t), "typedef tss_t differs from oracle");

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
