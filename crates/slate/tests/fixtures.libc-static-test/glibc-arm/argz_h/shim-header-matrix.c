#include <argz.h>

typedef int slate_oracle_typedef_error_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_error_t, error_t), "typedef error_t differs from oracle");

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

#ifndef ENOTSUP
#error "argz.h:ENOTSUP macro is missing from libc-shim"
#endif

int main(void) { return 0; }
