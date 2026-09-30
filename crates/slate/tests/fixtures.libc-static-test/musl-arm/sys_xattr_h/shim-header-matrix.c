#include <sys/xattr.h>

extern int slate_oracle_getxattr(const char *, const char *, void *, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getxattr), __typeof__(getxattr)),
    "sys/xattr.h:getxattr declaration differs from oracle");

static __typeof__(getxattr) *const slate_reference_getxattr = &getxattr;

#ifndef XATTR_CREATE
#error "sys/xattr.h:XATTR_CREATE macro is missing from libc-shim"
#endif

#ifndef XATTR_REPLACE
#error "sys/xattr.h:XATTR_REPLACE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
