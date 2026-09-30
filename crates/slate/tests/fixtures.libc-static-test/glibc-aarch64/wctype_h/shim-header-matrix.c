#include <wctype.h>

extern int slate_oracle_iswalnum_l(unsigned int, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_iswalnum_l), __typeof__(iswalnum_l)),
    "wctype.h:iswalnum_l declaration differs from oracle");

static __typeof__(iswalnum_l) *const slate_reference_iswalnum_l = &iswalnum_l;

typedef const int * slate_oracle_typedef_wctrans_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_wctrans_t, wctrans_t), "typedef wctrans_t differs from oracle");

typedef unsigned long slate_oracle_typedef_wctype_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_wctype_t, wctype_t), "typedef wctype_t differs from oracle");

#ifndef WEOF
#error "wctype.h:WEOF macro is missing from libc-shim"
#endif

int main(void) { return 0; }
