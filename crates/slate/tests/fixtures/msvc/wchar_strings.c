#include <wchar.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(WCHAR_MIN == 0, "WCHAR_MIN");
_Static_assert(WCHAR_MAX == 0xffff, "WCHAR_MAX");
_Static_assert(WEOF == (wint_t)0xffff, "WEOF");
_Static_assert(__builtin_types_compatible_p(_Wint_t, wchar_t), "_Wint_t");

TYPE_IS(&_wcsdup, wchar_t *(*)(const wchar_t *));
TYPE_IS(&wcscat, wchar_t *(*)(wchar_t *, const wchar_t *));
TYPE_IS(&wcscmp, int (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&wcscpy, wchar_t *(*)(wchar_t *, const wchar_t *));
TYPE_IS(&wcscspn, size_t (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&wcslen, size_t (*)(const wchar_t *));
TYPE_IS(&wcsnlen, size_t (*)(const wchar_t *, size_t));
TYPE_IS(&wcsncat, wchar_t *(*)(wchar_t *, const wchar_t *, size_t));
TYPE_IS(&wcsncmp, int (*)(const wchar_t *, const wchar_t *, size_t));
TYPE_IS(&wcsncpy, wchar_t *(*)(wchar_t *, const wchar_t *, size_t));
TYPE_IS(&wcspbrk, wchar_t *(*)(const wchar_t *, const wchar_t *));
TYPE_IS(&wcsspn, size_t (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&wcstok, wchar_t *(*)(wchar_t *, const wchar_t *, wchar_t **));
TYPE_IS(&wcschr, wchar_t *(*)(const wchar_t *, wchar_t));
TYPE_IS(&wcsrchr, wchar_t *(*)(const wchar_t *, wchar_t));
TYPE_IS(&wcsstr, wchar_t *(*)(const wchar_t *, const wchar_t *));
TYPE_IS(&wmemchr, wchar_t *(*)(const wchar_t *, wchar_t, size_t));
TYPE_IS(&wmemcmp, int (*)(const wchar_t *, const wchar_t *, size_t));
TYPE_IS(&wmemcpy, wchar_t *(*)(wchar_t *, const wchar_t *, size_t));
TYPE_IS(&wmemmove, wchar_t *(*)(wchar_t *, const wchar_t *, size_t));
TYPE_IS(&wmemset, wchar_t *(*)(wchar_t *, wchar_t, size_t));

TYPE_IS(&_wcserror, wchar_t *(*)(int));
TYPE_IS(&__wcserror, wchar_t *(*)(const wchar_t *));
TYPE_IS(&_wcsicmp, int (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&_wcsicmp_l,
        int (*)(const wchar_t *, const wchar_t *, _locale_t));
TYPE_IS(&_wcsnicmp, int (*)(const wchar_t *, const wchar_t *, size_t));
TYPE_IS(&_wcsnicmp_l,
        int (*)(const wchar_t *, const wchar_t *, size_t, _locale_t));
TYPE_IS(&_wcsnset, wchar_t *(*)(wchar_t *, wchar_t, size_t));
TYPE_IS(&_wcsrev, wchar_t *(*)(wchar_t *));
TYPE_IS(&_wcsset, wchar_t *(*)(wchar_t *, wchar_t));
TYPE_IS(&_wcslwr, wchar_t *(*)(wchar_t *));
TYPE_IS(&_wcslwr_l, wchar_t *(*)(wchar_t *, _locale_t));
TYPE_IS(&_wcsupr, wchar_t *(*)(wchar_t *));
TYPE_IS(&_wcsupr_l, wchar_t *(*)(wchar_t *, _locale_t));
TYPE_IS(&wcsxfrm, size_t (*)(wchar_t *, const wchar_t *, size_t));
TYPE_IS(&_wcsxfrm_l,
        size_t (*)(wchar_t *, const wchar_t *, size_t, _locale_t));
TYPE_IS(&wcscoll, int (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&_wcscoll_l,
        int (*)(const wchar_t *, const wchar_t *, _locale_t));
TYPE_IS(&_wcsicoll, int (*)(const wchar_t *, const wchar_t *));
TYPE_IS(&_wcsicoll_l,
        int (*)(const wchar_t *, const wchar_t *, _locale_t));
TYPE_IS(&_wcsncoll, int (*)(const wchar_t *, const wchar_t *, size_t));
TYPE_IS(&_wcsncoll_l,
        int (*)(const wchar_t *, const wchar_t *, size_t, _locale_t));
TYPE_IS(&_wcsnicoll, int (*)(const wchar_t *, const wchar_t *, size_t));
TYPE_IS(&_wcsnicoll_l,
        int (*)(const wchar_t *, const wchar_t *, size_t, _locale_t));

extern int wcwidth;
extern int wcswidth;

int main(void) { return 0; }
