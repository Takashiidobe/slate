#include <wctype.h>

typedef const int * slate_oracle_typedef_wctrans_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_wctrans_t, wctrans_t), "typedef wctrans_t differs from oracle");

#ifndef WEOF
#error "wctype.h:WEOF macro is missing from libc-shim"
#endif

#ifndef iswdigit
#error "wctype.h:iswdigit macro is missing from libc-shim"
#endif

int main(void) { return 0; }
