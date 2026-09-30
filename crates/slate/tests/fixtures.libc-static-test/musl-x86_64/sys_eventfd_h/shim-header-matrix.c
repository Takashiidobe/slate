#include <sys/eventfd.h>

typedef unsigned long slate_oracle_typedef_eventfd_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_eventfd_t, eventfd_t), "typedef eventfd_t differs from oracle");

#ifndef EFD_CLOEXEC
#error "sys/eventfd.h:EFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef EFD_NONBLOCK
#error "sys/eventfd.h:EFD_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef EFD_SEMAPHORE
#error "sys/eventfd.h:EFD_SEMAPHORE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
