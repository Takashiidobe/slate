#define __STDC_WANT_SECURE_LIB__ 0

#include <errno.h>
#include <locale.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

typedef int rsize_t;

extern int asctime_s;
extern int bsearch_s;
extern int clearerr_s;
extern int fopen_s;
extern int getenv_s;
extern int memcpy_s;
extern int printf_s;
extern int qsort_s;
extern int strcpy_s;
extern int wcscpy_s;
extern int wctomb_s;
extern int wmemcpy_s;
extern int wprintf_s;

TYPE_IS(&_strlwr_s, errno_t (*)(char *, size_t));
TYPE_IS(&_snprintf_s,
        int (*)(char *, size_t, size_t, const char *, ...));
TYPE_IS(&_itoa_s, errno_t (*)(int, char *, size_t, int));
TYPE_IS(&mbstowcs_s,
        errno_t (*)(size_t *, wchar_t *, size_t, const char *, size_t));
TYPE_IS(&_dupenv_s, errno_t (*)(char **, size_t *, const char *));
TYPE_IS(&mbsrtowcs_s,
        errno_t (*)(size_t *, wchar_t *, size_t, const char **, size_t,
                    mbstate_t *));
TYPE_IS(&wcrtomb_s,
        errno_t (*)(size_t *, char *, size_t, wchar_t, mbstate_t *));
TYPE_IS(&_ctime64_s,
        errno_t (*)(char *, size_t, const __time64_t *));

int main(void) { return 0; }
