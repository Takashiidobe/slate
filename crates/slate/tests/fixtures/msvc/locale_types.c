#include <locale.h>
#include <stddef.h>
#include <wctype.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(LC_ALL == 0, "LC_ALL");
_Static_assert(LC_COLLATE == 1, "LC_COLLATE");
_Static_assert(LC_CTYPE == 2, "LC_CTYPE");
_Static_assert(LC_MONETARY == 3, "LC_MONETARY");
_Static_assert(LC_NUMERIC == 4, "LC_NUMERIC");
_Static_assert(LC_TIME == 5, "LC_TIME");
_Static_assert(LC_MIN == LC_ALL, "LC_MIN");
_Static_assert(LC_MAX == LC_TIME, "LC_MAX");

_Static_assert(_ENABLE_PER_THREAD_LOCALE == 0x0001,
               "_ENABLE_PER_THREAD_LOCALE");
_Static_assert(_DISABLE_PER_THREAD_LOCALE == 0x0002,
               "_DISABLE_PER_THREAD_LOCALE");
_Static_assert(_ENABLE_PER_THREAD_LOCALE_GLOBAL == 0x0010,
               "_ENABLE_PER_THREAD_LOCALE_GLOBAL");
_Static_assert(_DISABLE_PER_THREAD_LOCALE_GLOBAL == 0x0020,
               "_DISABLE_PER_THREAD_LOCALE_GLOBAL");
_Static_assert(_ENABLE_PER_THREAD_LOCALE_NEW == 0x0100,
               "_ENABLE_PER_THREAD_LOCALE_NEW");
_Static_assert(_DISABLE_PER_THREAD_LOCALE_NEW == 0x0200,
               "_DISABLE_PER_THREAD_LOCALE_NEW");

_Static_assert(sizeof(wint_t) == 2, "wint_t size");
_Static_assert(sizeof(wctype_t) == 2, "wctype_t size");
_Static_assert(sizeof(wctrans_t) == 2, "wctrans_t size");
_Static_assert((wint_t)-1 > 0, "wint_t signedness");
_Static_assert((wctype_t)-1 > 0, "wctype_t signedness");
_Static_assert((wctrans_t)-1 > 0, "wctrans_t signedness");

_Static_assert(__builtin_types_compatible_p(mbstate_t, struct _Mbstatet),
               "mbstate_t tag");
_Static_assert(__builtin_types_compatible_p(mbstate_t, _Mbstatet),
               "_Mbstatet typedef");
_Static_assert(sizeof(mbstate_t) == 8, "mbstate_t size");
_Static_assert(_Alignof(mbstate_t) == 4, "mbstate_t alignment");
_Static_assert(offsetof(mbstate_t, _Wchar) == 0, "mbstate_t _Wchar");
_Static_assert(offsetof(mbstate_t, _Byte) == 4, "mbstate_t _Byte");
_Static_assert(offsetof(mbstate_t, _State) == 6, "mbstate_t _State");

_Static_assert(sizeof(__crt_locale_data_public) == 16,
               "__crt_locale_data_public size");
_Static_assert(_Alignof(__crt_locale_data_public) == 8,
               "__crt_locale_data_public alignment");
_Static_assert(offsetof(__crt_locale_data_public, _locale_pctype) == 0,
               "_locale_pctype");
_Static_assert(offsetof(__crt_locale_data_public, _locale_mb_cur_max) == 8,
               "_locale_mb_cur_max");
_Static_assert(offsetof(__crt_locale_data_public, _locale_lc_codepage) == 12,
               "_locale_lc_codepage");

_Static_assert(sizeof(__crt_locale_pointers) == 16,
               "__crt_locale_pointers size");
_Static_assert(_Alignof(__crt_locale_pointers) == 8,
               "__crt_locale_pointers alignment");
_Static_assert(offsetof(__crt_locale_pointers, locinfo) == 0, "locinfo");
_Static_assert(offsetof(__crt_locale_pointers, mbcinfo) == 8, "mbcinfo");
_Static_assert(__builtin_types_compatible_p(_locale_t,
                                            __crt_locale_pointers *),
               "_locale_t");

_Static_assert(sizeof(struct lconv) == 152, "lconv size");
_Static_assert(_Alignof(struct lconv) == 8, "lconv alignment");
_Static_assert(offsetof(struct lconv, decimal_point) == 0, "decimal_point");
_Static_assert(offsetof(struct lconv, negative_sign) == 72, "negative_sign");
_Static_assert(offsetof(struct lconv, int_frac_digits) == 80,
               "int_frac_digits");
_Static_assert(offsetof(struct lconv, n_sign_posn) == 87, "n_sign_posn");
_Static_assert(offsetof(struct lconv, _W_decimal_point) == 88,
               "_W_decimal_point");
_Static_assert(offsetof(struct lconv, _W_negative_sign) == 144,
               "_W_negative_sign");

TYPE_IS(&setlocale, char *(*)(int, const char *));
TYPE_IS(&localeconv, struct lconv *(*)(void));
TYPE_IS(&_lock_locales, void (*)(void));
TYPE_IS(&_unlock_locales, void (*)(void));
TYPE_IS(&_configthreadlocale, int (*)(int));
TYPE_IS(&_get_current_locale, _locale_t(*)(void));
TYPE_IS(&_create_locale, _locale_t(*)(int, const char *));
TYPE_IS(&_free_locale, void (*)(_locale_t));
TYPE_IS(&_wsetlocale, wchar_t *(*)(int, const wchar_t *));
TYPE_IS(&_wcreate_locale, _locale_t(*)(int, const wchar_t *));
TYPE_IS(&___lc_locale_name_func, wchar_t **(*)(void));
TYPE_IS(&___lc_codepage_func, unsigned int (*)(void));
TYPE_IS(&___lc_collate_cp_func, unsigned int (*)(void));

int main(void) { return 0; }
