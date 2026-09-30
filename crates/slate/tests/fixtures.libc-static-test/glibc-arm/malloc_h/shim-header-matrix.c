#include <malloc.h>

extern void * slate_oracle_malloc(__SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_malloc), __typeof__(malloc)),
    "malloc.h:malloc declaration differs from oracle");

static __typeof__(malloc) *const slate_reference_malloc = &malloc;

#ifndef M_ARENA_MAX
#error "malloc.h:M_ARENA_MAX macro is missing from libc-shim"
#endif

#ifndef M_ARENA_TEST
#error "malloc.h:M_ARENA_TEST macro is missing from libc-shim"
#endif

#ifndef M_CHECK_ACTION
#error "malloc.h:M_CHECK_ACTION macro is missing from libc-shim"
#endif

#ifndef M_GRAIN
#error "malloc.h:M_GRAIN macro is missing from libc-shim"
#endif

#ifndef M_KEEP
#error "malloc.h:M_KEEP macro is missing from libc-shim"
#endif

#ifndef M_MMAP_MAX
#error "malloc.h:M_MMAP_MAX macro is missing from libc-shim"
#endif

#ifndef M_MMAP_THRESHOLD
#error "malloc.h:M_MMAP_THRESHOLD macro is missing from libc-shim"
#endif

#ifndef M_MXFAST
#error "malloc.h:M_MXFAST macro is missing from libc-shim"
#endif

#ifndef M_NLBLKS
#error "malloc.h:M_NLBLKS macro is missing from libc-shim"
#endif

#ifndef M_PERTURB
#error "malloc.h:M_PERTURB macro is missing from libc-shim"
#endif

#ifndef M_TOP_PAD
#error "malloc.h:M_TOP_PAD macro is missing from libc-shim"
#endif

#ifndef M_TRIM_THRESHOLD
#error "malloc.h:M_TRIM_THRESHOLD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
