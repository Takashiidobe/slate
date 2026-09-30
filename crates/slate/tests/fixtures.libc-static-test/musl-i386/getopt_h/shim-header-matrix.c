#include <getopt.h>

extern int slate_oracle_getopt(int, char *const *, const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getopt), __typeof__(getopt)),
    "getopt.h:getopt declaration differs from oracle");

static __typeof__(getopt) *const slate_reference_getopt = &getopt;

#ifndef no_argument
#error "getopt.h:no_argument macro is missing from libc-shim"
#endif

#ifndef optional_argument
#error "getopt.h:optional_argument macro is missing from libc-shim"
#endif

#ifndef required_argument
#error "getopt.h:required_argument macro is missing from libc-shim"
#endif

int main(void) { return 0; }
