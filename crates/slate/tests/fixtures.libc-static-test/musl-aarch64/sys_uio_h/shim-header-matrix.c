#include <sys/uio.h>

extern long slate_oracle_readv(int, const struct iovec *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_readv), __typeof__(readv)),
    "sys/uio.h:readv declaration differs from oracle");

static __typeof__(readv) *const slate_reference_readv = &readv;

#ifndef RWF_APPEND
#error "sys/uio.h:RWF_APPEND macro is missing from libc-shim"
#endif

#ifndef RWF_DSYNC
#error "sys/uio.h:RWF_DSYNC macro is missing from libc-shim"
#endif

#ifndef RWF_HIPRI
#error "sys/uio.h:RWF_HIPRI macro is missing from libc-shim"
#endif

#ifndef RWF_NOAPPEND
#error "sys/uio.h:RWF_NOAPPEND macro is missing from libc-shim"
#endif

#ifndef RWF_NOWAIT
#error "sys/uio.h:RWF_NOWAIT macro is missing from libc-shim"
#endif

#ifndef RWF_SYNC
#error "sys/uio.h:RWF_SYNC macro is missing from libc-shim"
#endif

#ifndef UIO_MAXIOV
#error "sys/uio.h:UIO_MAXIOV macro is missing from libc-shim"
#endif

int main(void) { return 0; }
