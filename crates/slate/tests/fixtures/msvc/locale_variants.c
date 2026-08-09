#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

TYPE_IS(&_chvalidchk_l, int (*)(int, int, _locale_t));
TYPE_IS(&_ischartype_l, int (*)(int, int, _locale_t));
TYPE_IS(&_isctype_l, int (*)(int, int, _locale_t));
TYPE_IS(&_isalnum_l, int (*)(int, _locale_t));
TYPE_IS(&_isalpha_l, int (*)(int, _locale_t));
TYPE_IS(&_isblank_l, int (*)(int, _locale_t));
TYPE_IS(&_iscntrl_l, int (*)(int, _locale_t));
TYPE_IS(&_isdigit_l, int (*)(int, _locale_t));
TYPE_IS(&_isgraph_l, int (*)(int, _locale_t));
TYPE_IS(&_isleadbyte_l, int (*)(int, _locale_t));
TYPE_IS(&_islower_l, int (*)(int, _locale_t));
TYPE_IS(&_isprint_l, int (*)(int, _locale_t));
TYPE_IS(&_ispunct_l, int (*)(int, _locale_t));
TYPE_IS(&_isspace_l, int (*)(int, _locale_t));
TYPE_IS(&_isupper_l, int (*)(int, _locale_t));
TYPE_IS(&_isxdigit_l, int (*)(int, _locale_t));
TYPE_IS(&_tolower_l, int (*)(int, _locale_t));
TYPE_IS(&_toupper_l, int (*)(int, _locale_t));

TYPE_IS(&_memicmp_l,
        int (*)(const void *, const void *, size_t, _locale_t));
TYPE_IS(&_strcoll_l, int (*)(const char *, const char *, _locale_t));
TYPE_IS(&_stricmp_l, int (*)(const char *, const char *, _locale_t));
TYPE_IS(&_stricoll_l, int (*)(const char *, const char *, _locale_t));
TYPE_IS(&_strlwr_l, char *(*)(char *, _locale_t));
TYPE_IS(&_strncoll_l,
        int (*)(const char *, const char *, size_t, _locale_t));
TYPE_IS(&_strnicmp_l,
        int (*)(const char *, const char *, size_t, _locale_t));
TYPE_IS(&_strnicoll_l,
        int (*)(const char *, const char *, size_t, _locale_t));
TYPE_IS(&_strupr_l, char *(*)(char *, _locale_t));
TYPE_IS(&_strxfrm_l,
        size_t (*)(char *, const char *, size_t, _locale_t));

TYPE_IS(&_atof_l, double (*)(const char *, _locale_t));
TYPE_IS(&_atoi_l, int (*)(const char *, _locale_t));
TYPE_IS(&_atol_l, long (*)(const char *, _locale_t));
TYPE_IS(&_atoll_l, long long (*)(const char *, _locale_t));
TYPE_IS(&_atoi64_l, long long (*)(const char *, _locale_t));
TYPE_IS(&_strtod_l, double (*)(const char *, char **, _locale_t));
TYPE_IS(&_strtof_l, float (*)(const char *, char **, _locale_t));
TYPE_IS(&_strtold_l, long double (*)(const char *, char **, _locale_t));
TYPE_IS(&_strtol_l, long (*)(const char *, char **, int, _locale_t));
TYPE_IS(&_strtoul_l,
        unsigned long (*)(const char *, char **, int, _locale_t));
TYPE_IS(&_strtoll_l, long long (*)(const char *, char **, int, _locale_t));
TYPE_IS(&_strtoull_l,
        unsigned long long (*)(const char *, char **, int, _locale_t));
TYPE_IS(&_strtoi64_l,
        long long (*)(const char *, char **, int, _locale_t));
TYPE_IS(&_strtoui64_l,
        unsigned long long (*)(const char *, char **, int, _locale_t));

TYPE_IS(&_vfprintf_l, int (*)(FILE *, const char *, _locale_t, va_list));
TYPE_IS(&_vfprintf_p_l,
        int (*)(FILE *, const char *, _locale_t, va_list));
TYPE_IS(&_vprintf_l, int (*)(const char *, _locale_t, va_list));
TYPE_IS(&_vprintf_p_l, int (*)(const char *, _locale_t, va_list));
TYPE_IS(&_fprintf_l, int (*)(FILE *, const char *, _locale_t, ...));
TYPE_IS(&_fprintf_p_l, int (*)(FILE *, const char *, _locale_t, ...));
TYPE_IS(&_printf_l, int (*)(const char *, _locale_t, ...));
TYPE_IS(&_printf_p_l, int (*)(const char *, _locale_t, ...));

TYPE_IS(&_vfscanf_l, int (*)(FILE *, const char *, _locale_t, va_list));
TYPE_IS(&_vscanf_l, int (*)(const char *, _locale_t, va_list));
TYPE_IS(&_fscanf_l, int (*)(FILE *, const char *, _locale_t, ...));
TYPE_IS(&_scanf_l, int (*)(const char *, _locale_t, ...));

TYPE_IS(&_vscprintf_l, int (*)(const char *, _locale_t, va_list));
TYPE_IS(&_vscprintf_p_l, int (*)(const char *, _locale_t, va_list));
TYPE_IS(&_scprintf_l, int (*)(const char *, _locale_t, ...));
TYPE_IS(&_scprintf_p_l, int (*)(const char *, _locale_t, ...));
TYPE_IS(&_vsnprintf_l,
        int (*)(char *, size_t, const char *, _locale_t, va_list));
TYPE_IS(&_vsnprintf_c_l,
        int (*)(char *, size_t, const char *, _locale_t, va_list));
TYPE_IS(&_snprintf_l,
        int (*)(char *, size_t, const char *, _locale_t, ...));
TYPE_IS(&_snprintf_c_l,
        int (*)(char *, size_t, const char *, _locale_t, ...));
TYPE_IS(&_vsprintf_l,
        int (*)(char *, const char *, _locale_t, va_list));
TYPE_IS(&_vsprintf_p_l,
        int (*)(char *, size_t, const char *, _locale_t, va_list));
TYPE_IS(&_sprintf_l, int (*)(char *, const char *, _locale_t, ...));
TYPE_IS(&_sprintf_p_l,
        int (*)(char *, size_t, const char *, _locale_t, ...));
TYPE_IS(&_vsscanf_l,
        int (*)(const char *, const char *, _locale_t, va_list));
TYPE_IS(&_sscanf_l, int (*)(const char *, const char *, _locale_t, ...));
TYPE_IS(&_snscanf_l,
        int (*)(const char *, size_t, const char *, _locale_t, ...));

int main(void) { return 0; }
