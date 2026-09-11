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

_Static_assert(__STDC_WANT_SECURE_LIB__ == 1, "secure default");
_Static_assert(sizeof(rsize_t) == sizeof(size_t), "rsize_t size");
_Static_assert((rsize_t)-1 > 0, "rsize_t signedness");

TYPE_IS(&_set_errno, errno_t (*)(int));
TYPE_IS(&_get_errno, errno_t (*)(int *));
TYPE_IS(&__doserrno, unsigned long *(*)(void));
TYPE_IS(&_set_doserrno, errno_t (*)(unsigned long));
TYPE_IS(&_get_doserrno, errno_t (*)(unsigned long *));

TYPE_IS(&_set_invalid_parameter_handler,
        _invalid_parameter_handler (*)(_invalid_parameter_handler));
TYPE_IS(&_get_invalid_parameter_handler, _invalid_parameter_handler (*)(void));
TYPE_IS(&_set_thread_local_invalid_parameter_handler,
        _invalid_parameter_handler (*)(_invalid_parameter_handler));
TYPE_IS(&_get_thread_local_invalid_parameter_handler,
        _invalid_parameter_handler (*)(void));
TYPE_IS(&_invalid_parameter_noinfo, void (*)(void));
TYPE_IS(&_invalid_parameter_noinfo_noreturn, void (*)(void));
TYPE_IS(&_invoke_watson, void (*)(const wchar_t *, const wchar_t *,
                                  const wchar_t *, unsigned int, uintptr_t));

TYPE_IS(&memcpy_s, errno_t (*)(void *, rsize_t, const void *, rsize_t));
TYPE_IS(&memmove_s, errno_t (*)(void *, rsize_t, const void *, rsize_t));
TYPE_IS(&strcpy_s, errno_t (*)(char *, rsize_t, const char *));
TYPE_IS(&strncat_s, errno_t (*)(char *, rsize_t, const char *, rsize_t));
TYPE_IS(&strtok_s, char *(*)(char *, const char *, char **));
TYPE_IS(&_strlwr_s_l, errno_t (*)(char *, size_t, _locale_t));

TYPE_IS(&clearerr_s, errno_t (*)(FILE *));
TYPE_IS(&fopen_s, errno_t (*)(FILE **, const char *, const char *));
TYPE_IS(&fread_s, size_t (*)(void *, size_t, size_t, size_t, FILE *));
TYPE_IS(&printf_s, int (*)(const char *, ...));
TYPE_IS(&vfprintf_s, int (*)(FILE *, const char *, va_list));
TYPE_IS(&_snprintf_s, int (*)(char *, size_t, size_t, const char *, ...));
TYPE_IS(&_printf_s_l, int (*)(const char *, _locale_t, ...));
TYPE_IS(&_wfopen_s, errno_t (*)(FILE **, const wchar_t *, const wchar_t *));
TYPE_IS(&wprintf_s, int (*)(const wchar_t *, ...));
TYPE_IS(&_snwprintf_s_l,
        int (*)(wchar_t *, size_t, size_t, const wchar_t *, _locale_t, ...));

TYPE_IS(&bsearch_s,
        void *(*)(const void *, const void *, rsize_t, rsize_t,
                  int (*)(void *, const void *, const void *), void *));
TYPE_IS(&qsort_s,
        void (*)(void *, rsize_t, rsize_t,
                 int (*)(void *, const void *, const void *), void *));
TYPE_IS(&_itoa_s, errno_t (*)(int, char *, size_t, int));
TYPE_IS(&_itow_s, errno_t (*)(int, wchar_t *, size_t, int));
TYPE_IS(&mbstowcs_s,
        errno_t (*)(size_t *, wchar_t *, size_t, const char *, size_t));
TYPE_IS(&wctomb_s, errno_t (*)(int *, char *, rsize_t, wchar_t));
TYPE_IS(&getenv_s, errno_t (*)(size_t *, char *, rsize_t, const char *));
TYPE_IS(&_dupenv_s, errno_t (*)(char **, size_t *, const char *));
TYPE_IS(&_wgetenv_s, errno_t (*)(size_t *, wchar_t *, size_t, const wchar_t *));

TYPE_IS(&wcscpy_s, errno_t (*)(wchar_t *, rsize_t, const wchar_t *));
TYPE_IS(&wcsncpy_s, errno_t (*)(wchar_t *, rsize_t, const wchar_t *, rsize_t));
TYPE_IS(&wmemcpy_s, errno_t (*)(wchar_t *, rsize_t, const wchar_t *, rsize_t));
TYPE_IS(&mbsrtowcs_s, errno_t (*)(size_t *, wchar_t *, size_t, const char **,
                                  size_t, mbstate_t *));
TYPE_IS(&wcrtomb_s,
        errno_t (*)(size_t *, char *, size_t, wchar_t, mbstate_t *));

TYPE_IS(&asctime_s, errno_t (*)(char *, size_t, const struct tm *));
TYPE_IS(&ctime_s, errno_t (*)(char *, size_t, const time_t *));
TYPE_IS(&gmtime_s, errno_t (*)(struct tm *, const time_t *));
TYPE_IS(&_ctime32_s, errno_t (*)(char *, size_t, const __time32_t *));
TYPE_IS(&_wctime64_s, errno_t (*)(wchar_t *, size_t, const __time64_t *));

int main(void) { return 0; }
