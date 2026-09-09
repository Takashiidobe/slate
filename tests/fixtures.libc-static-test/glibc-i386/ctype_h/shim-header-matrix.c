#include <ctype.h>

extern int slate_oracle_isalnum_l(int, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalnum_l), __typeof__(isalnum_l)),
    "ctype.h:isalnum_l declaration differs from oracle");

static __typeof__(isalnum_l) *const slate_reference_isalnum_l = &isalnum_l;

#ifndef isalnum
#error "ctype.h:isalnum macro is missing from libc-shim"
#endif

#ifndef isalnum_l
#error "ctype.h:isalnum_l macro is missing from libc-shim"
#endif

#ifndef isalpha
#error "ctype.h:isalpha macro is missing from libc-shim"
#endif

#ifndef isalpha_l
#error "ctype.h:isalpha_l macro is missing from libc-shim"
#endif

#ifndef isascii
#error "ctype.h:isascii macro is missing from libc-shim"
#endif

#ifndef isascii_l
#error "ctype.h:isascii_l macro is missing from libc-shim"
#endif

#ifndef isblank
#error "ctype.h:isblank macro is missing from libc-shim"
#endif

#ifndef isblank_l
#error "ctype.h:isblank_l macro is missing from libc-shim"
#endif

#ifndef iscntrl
#error "ctype.h:iscntrl macro is missing from libc-shim"
#endif

#ifndef iscntrl_l
#error "ctype.h:iscntrl_l macro is missing from libc-shim"
#endif

#ifndef isdigit
#error "ctype.h:isdigit macro is missing from libc-shim"
#endif

#ifndef isdigit_l
#error "ctype.h:isdigit_l macro is missing from libc-shim"
#endif

#ifndef isgraph
#error "ctype.h:isgraph macro is missing from libc-shim"
#endif

#ifndef isgraph_l
#error "ctype.h:isgraph_l macro is missing from libc-shim"
#endif

#ifndef islower
#error "ctype.h:islower macro is missing from libc-shim"
#endif

#ifndef islower_l
#error "ctype.h:islower_l macro is missing from libc-shim"
#endif

#ifndef isprint
#error "ctype.h:isprint macro is missing from libc-shim"
#endif

#ifndef isprint_l
#error "ctype.h:isprint_l macro is missing from libc-shim"
#endif

#ifndef ispunct
#error "ctype.h:ispunct macro is missing from libc-shim"
#endif

#ifndef ispunct_l
#error "ctype.h:ispunct_l macro is missing from libc-shim"
#endif

#ifndef isspace
#error "ctype.h:isspace macro is missing from libc-shim"
#endif

#ifndef isspace_l
#error "ctype.h:isspace_l macro is missing from libc-shim"
#endif

#ifndef isupper
#error "ctype.h:isupper macro is missing from libc-shim"
#endif

#ifndef isupper_l
#error "ctype.h:isupper_l macro is missing from libc-shim"
#endif

#ifndef isxdigit
#error "ctype.h:isxdigit macro is missing from libc-shim"
#endif

#ifndef isxdigit_l
#error "ctype.h:isxdigit_l macro is missing from libc-shim"
#endif

#ifndef toascii
#error "ctype.h:toascii macro is missing from libc-shim"
#endif

#ifndef toascii_l
#error "ctype.h:toascii_l macro is missing from libc-shim"
#endif

int main(void) { return 0; }
