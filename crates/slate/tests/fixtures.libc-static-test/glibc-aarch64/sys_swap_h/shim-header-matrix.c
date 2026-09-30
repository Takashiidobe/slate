#include <sys/swap.h>

extern int slate_oracle_swapon(const char *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_swapon), __typeof__(swapon)),
    "sys/swap.h:swapon declaration differs from oracle");

static __typeof__(swapon) *const slate_reference_swapon = &swapon;

#ifndef SWAP_FLAG_DISCARD
#error "sys/swap.h:SWAP_FLAG_DISCARD macro is missing from libc-shim"
#endif

#ifndef SWAP_FLAG_PREFER
#error "sys/swap.h:SWAP_FLAG_PREFER macro is missing from libc-shim"
#endif

#ifndef SWAP_FLAG_PRIO_MASK
#error "sys/swap.h:SWAP_FLAG_PRIO_MASK macro is missing from libc-shim"
#endif

#ifndef SWAP_FLAG_PRIO_SHIFT
#error "sys/swap.h:SWAP_FLAG_PRIO_SHIFT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
