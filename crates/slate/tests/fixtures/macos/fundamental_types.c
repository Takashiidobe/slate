#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
_Static_assert(sizeof(long double) == 8, "long double");
_Static_assert(_Alignof(long double) == 8, "long double alignment");
_Static_assert(sizeof(max_align_t) == 8, "max_align_t");
_Static_assert(_Alignof(max_align_t) == 8, "max_align_t alignment");
_Static_assert(sizeof(time_t) == 8, "time_t");
_Static_assert(sizeof(clock_t) == 8, "clock_t");
_Static_assert((clock_t)-1 > 0, "clock_t signedness");
_Static_assert(sizeof(va_list) == 8, "va_list");
_Static_assert(__builtin_types_compatible_p(wchar_t, int), "wchar_t type");
_Static_assert(__builtin_types_compatible_p(time_t, long), "time_t type");
_Static_assert(__builtin_types_compatible_p(clock_t, unsigned long),
               "clock_t type");
_Static_assert(__builtin_types_compatible_p(va_list, char *), "va_list type");
_Static_assert(__builtin_types_compatible_p(fpos_t, long long), "fpos_t type");
_Static_assert(LDBL_MANT_DIG == 53, "long double mantissa");
_Static_assert(LDBL_MAX_EXP == 1024, "long double exponent");
_Static_assert(__builtin_types_compatible_p(int64_t, long long), "int64_t");
_Static_assert(__builtin_types_compatible_p(int_fast16_t, short), "int_fast16_t");
_Static_assert(__builtin_types_compatible_p(int_fast32_t, int), "int_fast32_t");
_Static_assert(__builtin_types_compatible_p(int_fast64_t, long long),
               "int_fast64_t");
_Static_assert(__builtin_types_compatible_p(intmax_t, long), "intmax_t");
_Static_assert(__builtin_types_compatible_p(uintmax_t, unsigned long),
               "uintmax_t");
_Static_assert(WCHAR_MIN == (-2147483647 - 1), "WCHAR_MIN");
_Static_assert(WCHAR_MAX == 2147483647, "WCHAR_MAX");
_Static_assert(EDOM == 33, "EDOM");
_Static_assert(ERANGE == 34, "ERANGE");
_Static_assert(EILSEQ == 92, "EILSEQ");
_Static_assert(FOPEN_MAX == 20, "FOPEN_MAX");
_Static_assert(FILENAME_MAX == 1024, "FILENAME_MAX");
_Static_assert(TMP_MAX == 308915776, "TMP_MAX");
_Static_assert(L_tmpnam == 1024, "L_tmpnam");
_Static_assert(CLOCKS_PER_SEC == 1000000L, "CLOCKS_PER_SEC");
_Static_assert(sizeof(struct tm) == 56, "struct tm");
_Static_assert(__builtin_types_compatible_p(__typeof__(&__error),
                                            int *(*)(void)),
               "__error signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&printf),
                                            int (*)(const char *, ...)),
               "printf signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&malloc),
                                            void *(*)(size_t)),
               "malloc signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&memcpy),
                                            void *(*)(void *, const void *,
                                                      size_t)),
               "memcpy signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&sin),
                                            double (*)(double)),
               "sin signature");

#if defined(__linux__) || defined(__GLIBC__) || defined(__MUSL__)
#error "Linux target state leaked into the Darwin profile"
#endif

extern long darwin_import(size_t, ptrdiff_t, intptr_t, uintptr_t, wchar_t,
                          time_t, clock_t, long double);

long call_darwin_import(size_t size, ptrdiff_t difference, intptr_t signed_ptr,
                        uintptr_t unsigned_ptr, wchar_t wide, time_t seconds,
                        clock_t ticks, long double real) {
  return darwin_import(size, difference, signed_ptr, unsigned_ptr, wide,
                       seconds, ticks, real);
}

int darwin_variadic_count(int count, ...) {
  va_list values;
  int total = 0;
  va_start(values, count);
  for (int i = 0; i < count; ++i)
    total += va_arg(values, int);
  va_end(values);
  return total;
}

int main(void) { return printf("%d\n", errno == 0); }
