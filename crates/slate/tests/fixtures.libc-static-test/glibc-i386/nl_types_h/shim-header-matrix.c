#include <nl_types.h>

typedef void * slate_oracle_typedef_nl_catd;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_nl_catd, nl_catd), "typedef nl_catd differs from oracle");

#ifndef NL_CAT_LOCALE
#error "nl_types.h:NL_CAT_LOCALE macro is missing from libc-shim"
#endif

#ifndef NL_SETD
#error "nl_types.h:NL_SETD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
