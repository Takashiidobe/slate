#include <pwd.h>

extern void slate_oracle_setpwent(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setpwent), __typeof__(setpwent)),
    "pwd.h:setpwent declaration differs from oracle");

static __typeof__(setpwent) *const slate_reference_setpwent = &setpwent;

typedef unsigned int slate_oracle_typedef_gid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_gid_t, gid_t), "typedef gid_t differs from oracle");

#ifndef NSS_BUFLEN_PASSWD
#error "pwd.h:NSS_BUFLEN_PASSWD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
