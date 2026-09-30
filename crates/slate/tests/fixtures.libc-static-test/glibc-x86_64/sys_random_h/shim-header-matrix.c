#include <sys/random.h>

extern long slate_oracle_getrandom(void *, unsigned long, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getrandom), __typeof__(getrandom)),
    "sys/random.h:getrandom declaration differs from oracle");

static __typeof__(getrandom) *const slate_reference_getrandom = &getrandom;

#ifndef GRND_INSECURE
#error "sys/random.h:GRND_INSECURE macro is missing from libc-shim"
#endif

#ifndef GRND_NONBLOCK
#error "sys/random.h:GRND_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef GRND_RANDOM
#error "sys/random.h:GRND_RANDOM macro is missing from libc-shim"
#endif

int main(void) { return 0; }
