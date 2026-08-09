#include <wchar.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

TYPE_IS(&fgetwc, wint_t (*)(FILE *));
TYPE_IS(&_fgetwchar, wint_t (*)(void));
TYPE_IS(&fputwc, wint_t (*)(wchar_t, FILE *));
TYPE_IS(&_fputwchar, wint_t (*)(wchar_t));
TYPE_IS(&getwc, wint_t (*)(FILE *));
TYPE_IS(&getwchar, wint_t (*)(void));
TYPE_IS(&fgetws, wchar_t *(*)(wchar_t *, int, FILE *));
TYPE_IS(&fputws, int (*)(const wchar_t *, FILE *));
TYPE_IS(&putwc, wint_t (*)(wchar_t, FILE *));
TYPE_IS(&putwchar, wint_t (*)(wchar_t));
TYPE_IS(&_putws, int (*)(const wchar_t *));
TYPE_IS(&ungetwc, wint_t (*)(wint_t, FILE *));
TYPE_IS(&_wfdopen, FILE *(*)(int, const wchar_t *));
TYPE_IS(&_wfopen, FILE *(*)(const wchar_t *, const wchar_t *));
TYPE_IS(&_wfreopen,
        FILE *(*)(const wchar_t *, const wchar_t *, FILE *));
TYPE_IS(&_wfsopen,
        FILE *(*)(const wchar_t *, const wchar_t *, int));
TYPE_IS(&_wperror, void (*)(const wchar_t *));
TYPE_IS(&_wpopen, FILE *(*)(const wchar_t *, const wchar_t *));
TYPE_IS(&_wremove, int (*)(const wchar_t *));
TYPE_IS(&_wtempnam, wchar_t *(*)(const wchar_t *, const wchar_t *));
TYPE_IS(&_wtmpnam, wchar_t *(*)(wchar_t *));
TYPE_IS(&_fgetwc_nolock, wint_t (*)(FILE *));
TYPE_IS(&_fputwc_nolock, wint_t (*)(wchar_t, FILE *));
TYPE_IS(&_getwc_nolock, wint_t (*)(FILE *));
TYPE_IS(&_putwc_nolock, wint_t (*)(wchar_t, FILE *));
TYPE_IS(&_ungetwc_nolock, wint_t (*)(wint_t, FILE *));

TYPE_IS(&vfwprintf, int (*)(FILE *, const wchar_t *, va_list));
TYPE_IS(&vwprintf, int (*)(const wchar_t *, va_list));
TYPE_IS(&_vfwprintf_l,
        int (*)(FILE *, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_vfwprintf_p, int (*)(FILE *, const wchar_t *, va_list));
TYPE_IS(&_vfwprintf_p_l,
        int (*)(FILE *, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_vwprintf_l, int (*)(const wchar_t *, _locale_t, va_list));
TYPE_IS(&_vwprintf_p, int (*)(const wchar_t *, va_list));
TYPE_IS(&_vwprintf_p_l,
        int (*)(const wchar_t *, _locale_t, va_list));
TYPE_IS(&fwprintf, int (*)(FILE *, const wchar_t *, ...));
TYPE_IS(&wprintf, int (*)(const wchar_t *, ...));
TYPE_IS(&_fwprintf_l,
        int (*)(FILE *, const wchar_t *, _locale_t, ...));
TYPE_IS(&_fwprintf_p, int (*)(FILE *, const wchar_t *, ...));
TYPE_IS(&_fwprintf_p_l,
        int (*)(FILE *, const wchar_t *, _locale_t, ...));
TYPE_IS(&_wprintf_l, int (*)(const wchar_t *, _locale_t, ...));
TYPE_IS(&_wprintf_p, int (*)(const wchar_t *, ...));
TYPE_IS(&_wprintf_p_l, int (*)(const wchar_t *, _locale_t, ...));

TYPE_IS(&vfwscanf, int (*)(FILE *, const wchar_t *, va_list));
TYPE_IS(&vwscanf, int (*)(const wchar_t *, va_list));
TYPE_IS(&_vfwscanf_l,
        int (*)(FILE *, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_vwscanf_l, int (*)(const wchar_t *, _locale_t, va_list));
TYPE_IS(&fwscanf, int (*)(FILE *, const wchar_t *, ...));
TYPE_IS(&wscanf, int (*)(const wchar_t *, ...));
TYPE_IS(&_fwscanf_l,
        int (*)(FILE *, const wchar_t *, _locale_t, ...));
TYPE_IS(&_wscanf_l, int (*)(const wchar_t *, _locale_t, ...));

TYPE_IS(&vswprintf,
        int (*)(wchar_t *, size_t, const wchar_t *, va_list));
TYPE_IS(&swprintf, int (*)(wchar_t *, size_t, const wchar_t *, ...));
TYPE_IS(&_vsnwprintf,
        int (*)(wchar_t *, size_t, const wchar_t *, va_list));
TYPE_IS(&_vsnwprintf_l,
        int (*)(wchar_t *, size_t, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_snwprintf, int (*)(wchar_t *, size_t, const wchar_t *, ...));
TYPE_IS(&_snwprintf_l,
        int (*)(wchar_t *, size_t, const wchar_t *, _locale_t, ...));
TYPE_IS(&_vswprintf, int (*)(wchar_t *, const wchar_t *, va_list));
TYPE_IS(&_vswprintf_l,
        int (*)(wchar_t *, size_t, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_swprintf, int (*)(wchar_t *, const wchar_t *, ...));
TYPE_IS(&_swprintf_l,
        int (*)(wchar_t *, size_t, const wchar_t *, _locale_t, ...));
TYPE_IS(&_vswprintf_p,
        int (*)(wchar_t *, size_t, const wchar_t *, va_list));
TYPE_IS(&_vswprintf_p_l,
        int (*)(wchar_t *, size_t, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_swprintf_p,
        int (*)(wchar_t *, size_t, const wchar_t *, ...));
TYPE_IS(&_swprintf_p_l,
        int (*)(wchar_t *, size_t, const wchar_t *, _locale_t, ...));
TYPE_IS(&_vscwprintf, int (*)(const wchar_t *, va_list));
TYPE_IS(&_vscwprintf_l,
        int (*)(const wchar_t *, _locale_t, va_list));
TYPE_IS(&_scwprintf, int (*)(const wchar_t *, ...));
TYPE_IS(&_scwprintf_l, int (*)(const wchar_t *, _locale_t, ...));

TYPE_IS(&vswscanf,
        int (*)(const wchar_t *, const wchar_t *, va_list));
TYPE_IS(&swscanf, int (*)(const wchar_t *, const wchar_t *, ...));
TYPE_IS(&_vswscanf_l,
        int (*)(const wchar_t *, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_swscanf_l,
        int (*)(const wchar_t *, const wchar_t *, _locale_t, ...));
TYPE_IS(&_vsnwscanf_l,
        int (*)(const wchar_t *, size_t, const wchar_t *, _locale_t, va_list));
TYPE_IS(&_snwscanf,
        int (*)(const wchar_t *, size_t, const wchar_t *, ...));
TYPE_IS(&_snwscanf_l,
        int (*)(const wchar_t *, size_t, const wchar_t *, _locale_t, ...));

int main(void) { return 0; }
