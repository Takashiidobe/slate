#include <ctype.h>

extern int slate_oracle_isalnum(int);
extern int slate_oracle_isalnum_l(int, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalnum), __typeof__(isalnum)),
    "ctype.h:isalnum declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalnum_l), __typeof__(isalnum_l)),
    "ctype.h:isalnum_l declaration differs from oracle");

static __typeof__(isalnum) *const slate_reference_isalnum = &isalnum;
static __typeof__(isalnum_l) *const slate_reference_isalnum_l = &isalnum_l;

#ifndef isalpha
#error "ctype.h:isalpha macro is missing from libc-shim"
#endif

#ifndef isascii
#error "ctype.h:isascii macro is missing from libc-shim"
#endif

#ifndef isdigit
#error "ctype.h:isdigit macro is missing from libc-shim"
#endif

#ifndef isgraph
#error "ctype.h:isgraph macro is missing from libc-shim"
#endif

#ifndef islower
#error "ctype.h:islower macro is missing from libc-shim"
#endif

#ifndef isprint
#error "ctype.h:isprint macro is missing from libc-shim"
#endif

#ifndef isspace
#error "ctype.h:isspace macro is missing from libc-shim"
#endif

#ifndef isupper
#error "ctype.h:isupper macro is missing from libc-shim"
#endif

int main(void) { return 0; }
