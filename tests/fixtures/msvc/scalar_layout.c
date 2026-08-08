#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(wchar_t) == 2, "wchar_t");
_Static_assert(sizeof(long) == 4, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(long double) == 8, "long double");
_Static_assert(LONG_MAX == 2147483647L, "LONG_MAX");
_Static_assert(WCHAR_MAX == 65535, "WCHAR_MAX");
_Static_assert(LDBL_MANT_DIG == 53, "LDBL_MANT_DIG");

int main(void) { return 0; }
