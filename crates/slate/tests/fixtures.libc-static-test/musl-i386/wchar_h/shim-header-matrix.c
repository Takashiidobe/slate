#include <wchar.h>

extern int * slate_oracle_wcscpy(int *restrict, const int *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcscpy), __typeof__(wcscpy)),
    "wchar.h:wcscpy declaration differs from oracle");

static __typeof__(wcscpy) *const slate_reference_wcscpy = &wcscpy;

#ifndef NULL
#error "wchar.h:NULL macro is missing from libc-shim"
#endif

#ifndef WCHAR_MAX
#error "wchar.h:WCHAR_MAX macro is missing from libc-shim"
#endif

#ifndef WCHAR_MIN
#error "wchar.h:WCHAR_MIN macro is missing from libc-shim"
#endif

#ifndef WEOF
#error "wchar.h:WEOF macro is missing from libc-shim"
#endif

#ifndef iswdigit
#error "wchar.h:iswdigit macro is missing from libc-shim"
#endif

int main(void) { return 0; }
