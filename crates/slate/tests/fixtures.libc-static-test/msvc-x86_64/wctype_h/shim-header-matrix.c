#include <wctype.h>

extern unsigned short slate_oracle_towctrans(unsigned short, unsigned short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_towctrans), __typeof__(towctrans)),
    "wctype.h:towctrans declaration differs from oracle");

static __typeof__(towctrans) *const slate_reference_towctrans = &towctrans;

extern unsigned short slate_oracle_wctrans(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wctrans), __typeof__(wctrans)),
    "wctype.h:wctrans declaration differs from oracle");

static __typeof__(wctrans) *const slate_reference_wctrans = &wctrans;

extern unsigned short slate_oracle_wctype(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wctype), __typeof__(wctype)),
    "wctype.h:wctype declaration differs from oracle");

static __typeof__(wctype) *const slate_reference_wctype = &wctype;

typedef unsigned short slate_oracle_typedef_wctrans_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_wctrans_t, wctrans_t), "typedef wctrans_t differs from oracle");

#ifndef _INC_WCTYPE
#error "wctype.h:_INC_WCTYPE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
