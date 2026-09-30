#include <ulimit.h>

extern long slate_oracle_ulimit(int, ...);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ulimit), __typeof__(ulimit)),
    "ulimit.h:ulimit declaration differs from oracle");

static __typeof__(ulimit) *const slate_reference_ulimit = &ulimit;

#ifndef UL_GETFSIZE
#error "ulimit.h:UL_GETFSIZE macro is missing from libc-shim"
#endif

#ifndef UL_SETFSIZE
#error "ulimit.h:UL_SETFSIZE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
