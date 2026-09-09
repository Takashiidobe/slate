#include <argz.h>

typedef int slate_oracle_typedef_error_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_error_t, error_t), "error_t typedef mismatch");

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "locale_t typedef mismatch");

#ifndef ENOTSUP
#error "missing macro ENOTSUP"
#endif

int main(void) { return 0; }
