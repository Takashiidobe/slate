#include <grp.h>

extern void slate_oracle_setgrent(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setgrent), __typeof__(setgrent)),
    "grp.h:setgrent declaration differs from oracle");

static __typeof__(setgrent) *const slate_reference_setgrent = &setgrent;

typedef unsigned int slate_oracle_typedef_gid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_gid_t, gid_t), "typedef gid_t differs from oracle");

#ifndef NSS_BUFLEN_GROUP
#error "grp.h:NSS_BUFLEN_GROUP macro is missing from libc-shim"
#endif

int main(void) { return 0; }
