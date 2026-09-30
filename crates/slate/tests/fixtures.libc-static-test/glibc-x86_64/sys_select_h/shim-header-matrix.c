#include <sys/select.h>

typedef long slate_oracle_typedef_suseconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_suseconds_t, suseconds_t), "typedef suseconds_t differs from oracle");

#ifndef FD_CLR
#error "sys/select.h:FD_CLR macro is missing from libc-shim"
#endif

#ifndef FD_ISSET
#error "sys/select.h:FD_ISSET macro is missing from libc-shim"
#endif

#ifndef FD_SET
#error "sys/select.h:FD_SET macro is missing from libc-shim"
#endif

#ifndef FD_SETSIZE
#error "sys/select.h:FD_SETSIZE macro is missing from libc-shim"
#endif

#ifndef FD_ZERO
#error "sys/select.h:FD_ZERO macro is missing from libc-shim"
#endif

#ifndef NFDBITS
#error "sys/select.h:NFDBITS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
