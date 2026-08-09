#include <locale.h>
#include <stdlib.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

TYPE_IS(MB_CUR_MAX, int);
TYPE_IS(__mb_cur_max, int);
TYPE_IS(&___mb_cur_max_func, int (*)(void));
TYPE_IS(&___mb_cur_max_l_func, int (*)(_locale_t));

TYPE_IS(&mblen, int (*)(const char *, size_t));
TYPE_IS(&_mblen_l, int (*)(const char *, size_t, _locale_t));
TYPE_IS(&_mbstrlen, size_t (*)(const char *));
TYPE_IS(&_mbstrlen_l, size_t (*)(const char *, _locale_t));
TYPE_IS(&_mbstrnlen, size_t (*)(const char *, size_t));
TYPE_IS(&_mbstrnlen_l, size_t (*)(const char *, size_t, _locale_t));
TYPE_IS(&mbtowc, int (*)(wchar_t *, const char *, size_t));
TYPE_IS(&_mbtowc_l, int (*)(wchar_t *, const char *, size_t, _locale_t));
TYPE_IS(&mbstowcs, size_t (*)(wchar_t *, const char *, size_t));
TYPE_IS(&_mbstowcs_l,
        size_t (*)(wchar_t *, const char *, size_t, _locale_t));
TYPE_IS(&wctomb, int (*)(char *, wchar_t));
TYPE_IS(&_wctomb_l, int (*)(char *, wchar_t, _locale_t));
TYPE_IS(&wcstombs, size_t (*)(char *, const wchar_t *, size_t));
TYPE_IS(&_wcstombs_l,
        size_t (*)(char *, const wchar_t *, size_t, _locale_t));

int main(void) { return 0; }
