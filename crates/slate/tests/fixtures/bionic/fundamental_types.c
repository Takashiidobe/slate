#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

_Static_assert(sizeof(char) == 1, "char");
_Static_assert(sizeof(short) == 2, "short");
_Static_assert(sizeof(int) == 4, "int");
_Static_assert(sizeof(long) == 8, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(void *) == 8, "pointer");
_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(intmax_t) == 8, "intmax_t");
_Static_assert(sizeof(uintmax_t) == 8, "uintmax_t");
_Static_assert(sizeof(wchar_t) == 4, "wchar_t");
_Static_assert(sizeof(wint_t) == 4, "wint_t");
_Static_assert(sizeof(long double) == 16, "long double");
_Static_assert(_Alignof(long double) == 16, "long double alignment");
_Static_assert(sizeof(max_align_t) == 32, "max_align_t");
_Static_assert(_Alignof(max_align_t) == 16, "max_align_t alignment");
_Static_assert(LDBL_MANT_DIG == 113, "long double mantissa");
_Static_assert(LDBL_MAX_EXP == 16384, "long double exponent");
_Static_assert(INTMAX_MAX == 9223372036854775807L, "INTMAX_MAX");
_Static_assert(UINTMAX_MAX == 18446744073709551615UL, "UINTMAX_MAX");
_Static_assert(WINT_MAX == 4294967295U, "WINT_MAX");

#if defined(__aarch64__)
_Static_assert(WCHAR_MAX == 4294967295U, "WCHAR_MAX");
_Static_assert(WCHAR_MIN == 0, "WCHAR_MIN");
_Static_assert(sizeof(va_list) == 32, "va_list");
#elif defined(__x86_64__)
_Static_assert(WCHAR_MAX == 2147483647, "WCHAR_MAX");
_Static_assert(WCHAR_MIN == (-2147483647 - 1), "WCHAR_MIN");
_Static_assert(sizeof(va_list) == 24, "va_list");
#else
#error "unsupported Bionic architecture"
#endif

extern long bionic_import(size_t, ptrdiff_t, intptr_t, uintptr_t, wchar_t,
                          wint_t, long double);

long call_bionic_import(size_t size, ptrdiff_t difference, intptr_t signed_ptr,
                        uintptr_t unsigned_ptr, wchar_t wide, wint_t wide_int,
                        long double real) {
  return bionic_import(size, difference, signed_ptr, unsigned_ptr, wide,
                       wide_int, real);
}

int bionic_variadic_count(int count, ...) {
  va_list values;
  int     total = 0;
  va_start(values, count);
  for (int i = 0; i < count; ++i)
    total += va_arg(values, int);
  va_end(values);
  return total;
}

int main(void) { return 0; }
