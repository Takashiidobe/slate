#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

TYPE_IS(&iswalpha, int (*)(wint_t));
TYPE_IS(&_iswalpha_l, int (*)(wint_t, _locale_t));
TYPE_IS(&fgetwc, wint_t (*)(FILE *));
TYPE_IS(&fwprintf, int (*)(FILE *, const wchar_t *, ...));
TYPE_IS(&wcstod, double (*)(const wchar_t *, wchar_t **));
TYPE_IS(&_wtoi, int (*)(const wchar_t *));
TYPE_IS(&wcscmp, int (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&wcslen, size_t (*)(const wchar_t *));

int main(void) { return 0; }
