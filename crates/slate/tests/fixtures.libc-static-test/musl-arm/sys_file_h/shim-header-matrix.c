#include <sys/file.h>

extern int slate_oracle_flock(int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_flock), __typeof__(flock)),
    "sys/file.h:flock declaration differs from oracle");

static __typeof__(flock) *const slate_reference_flock = &flock;

#ifndef LOCK_EX
#error "sys/file.h:LOCK_EX macro is missing from libc-shim"
#endif

#ifndef LOCK_NB
#error "sys/file.h:LOCK_NB macro is missing from libc-shim"
#endif

#ifndef LOCK_SH
#error "sys/file.h:LOCK_SH macro is missing from libc-shim"
#endif

#ifndef LOCK_UN
#error "sys/file.h:LOCK_UN macro is missing from libc-shim"
#endif

#ifndef L_INCR
#error "sys/file.h:L_INCR macro is missing from libc-shim"
#endif

#ifndef L_SET
#error "sys/file.h:L_SET macro is missing from libc-shim"
#endif

#ifndef L_XTND
#error "sys/file.h:L_XTND macro is missing from libc-shim"
#endif

int main(void) { return 0; }
