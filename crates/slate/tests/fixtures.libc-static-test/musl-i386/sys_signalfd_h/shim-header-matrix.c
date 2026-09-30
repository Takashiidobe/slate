#include <sys/signalfd.h>

extern int slate_oracle_signalfd(int, const struct __sigset_t *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_signalfd), __typeof__(signalfd)),
    "sys/signalfd.h:signalfd declaration differs from oracle");

static __typeof__(signalfd) *const slate_reference_signalfd = &signalfd;

#ifndef SFD_CLOEXEC
#error "sys/signalfd.h:SFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef SFD_NONBLOCK
#error "sys/signalfd.h:SFD_NONBLOCK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
