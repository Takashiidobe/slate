#include <wchar.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

TYPE_IS(&btowc, wint_t (*)(int));
TYPE_IS(&mbrlen, size_t (*)(const char *, size_t, mbstate_t *));
TYPE_IS(&mbrtowc,
        size_t (*)(wchar_t *, const char *, size_t, mbstate_t *));
TYPE_IS(&mbsrtowcs,
        size_t (*)(wchar_t *, const char **, size_t, mbstate_t *));
TYPE_IS(&wcrtomb, size_t (*)(char *, wchar_t, mbstate_t *));
TYPE_IS(&wcsrtombs,
        size_t (*)(char *, const wchar_t **, size_t, mbstate_t *));
TYPE_IS(&wctob, int (*)(wint_t));
TYPE_IS(&fwide, int (*)(FILE *, int));
TYPE_IS(&mbsinit, int (*)(const mbstate_t *));

TYPE_IS(&wcstod, double (*)(const wchar_t *, wchar_t **));
TYPE_IS(&_wcstod_l, double (*)(const wchar_t *, wchar_t **, _locale_t));
TYPE_IS(&wcstof, float (*)(const wchar_t *, wchar_t **));
TYPE_IS(&_wcstof_l, float (*)(const wchar_t *, wchar_t **, _locale_t));
TYPE_IS(&wcstold, long double (*)(const wchar_t *, wchar_t **));
TYPE_IS(&_wcstold_l,
        long double (*)(const wchar_t *, wchar_t **, _locale_t));
TYPE_IS(&wcstol, long (*)(const wchar_t *, wchar_t **, int));
TYPE_IS(&_wcstol_l,
        long (*)(const wchar_t *, wchar_t **, int, _locale_t));
TYPE_IS(&wcstoul, unsigned long (*)(const wchar_t *, wchar_t **, int));
TYPE_IS(&_wcstoul_l,
        unsigned long (*)(const wchar_t *, wchar_t **, int, _locale_t));
TYPE_IS(&wcstoll, long long (*)(const wchar_t *, wchar_t **, int));
TYPE_IS(&_wcstoll_l,
        long long (*)(const wchar_t *, wchar_t **, int, _locale_t));
TYPE_IS(&wcstoull,
        unsigned long long (*)(const wchar_t *, wchar_t **, int));
TYPE_IS(&_wcstoull_l,
        unsigned long long (*)(const wchar_t *, wchar_t **, int, _locale_t));

TYPE_IS(&_wtof, double (*)(const wchar_t *));
TYPE_IS(&_wtof_l, double (*)(const wchar_t *, _locale_t));
TYPE_IS(&_wtoi, int (*)(const wchar_t *));
TYPE_IS(&_wtoi_l, int (*)(const wchar_t *, _locale_t));
TYPE_IS(&_wtol, long (*)(const wchar_t *));
TYPE_IS(&_wtol_l, long (*)(const wchar_t *, _locale_t));
TYPE_IS(&_wtoll, long long (*)(const wchar_t *));
TYPE_IS(&_wtoll_l, long long (*)(const wchar_t *, _locale_t));
TYPE_IS(&_wtoi64, long long (*)(const wchar_t *));
TYPE_IS(&_wtoi64_l, long long (*)(const wchar_t *, _locale_t));
TYPE_IS(&_wcstoi64, long long (*)(const wchar_t *, wchar_t **, int));
TYPE_IS(&_wcstoi64_l,
        long long (*)(const wchar_t *, wchar_t **, int, _locale_t));
TYPE_IS(&_wcstoui64,
        unsigned long long (*)(const wchar_t *, wchar_t **, int));
TYPE_IS(&_wcstoui64_l,
        unsigned long long (*)(const wchar_t *, wchar_t **, int, _locale_t));
TYPE_IS(&_itow, wchar_t *(*)(int, wchar_t *, int));
TYPE_IS(&_ltow, wchar_t *(*)(long, wchar_t *, int));
TYPE_IS(&_ultow, wchar_t *(*)(unsigned long, wchar_t *, int));
TYPE_IS(&_i64tow, wchar_t *(*)(long long, wchar_t *, int));
TYPE_IS(&_ui64tow, wchar_t *(*)(unsigned long long, wchar_t *, int));

int main(void) { return 0; }
