#include <locale.h>

extern unsigned int slate_oracle____lc_codepage_func(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle____lc_codepage_func), __typeof__(___lc_codepage_func)),
    "locale.h:___lc_codepage_func declaration differs from oracle");

static __typeof__(___lc_codepage_func) *const slate_reference____lc_codepage_func = &___lc_codepage_func;

extern unsigned int slate_oracle____lc_collate_cp_func(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle____lc_collate_cp_func), __typeof__(___lc_collate_cp_func)),
    "locale.h:___lc_collate_cp_func declaration differs from oracle");

static __typeof__(___lc_collate_cp_func) *const slate_reference____lc_collate_cp_func = &___lc_collate_cp_func;

extern unsigned short ** slate_oracle____lc_locale_name_func(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle____lc_locale_name_func), __typeof__(___lc_locale_name_func)),
    "locale.h:___lc_locale_name_func declaration differs from oracle");

static __typeof__(___lc_locale_name_func) *const slate_reference____lc_locale_name_func = &___lc_locale_name_func;

extern int slate_oracle__configthreadlocale(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__configthreadlocale), __typeof__(_configthreadlocale)),
    "locale.h:_configthreadlocale declaration differs from oracle");

static __typeof__(_configthreadlocale) *const slate_reference__configthreadlocale = &_configthreadlocale;

extern _locale_t slate_oracle__create_locale(int, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__create_locale), __typeof__(_create_locale)),
    "locale.h:_create_locale declaration differs from oracle");

static __typeof__(_create_locale) *const slate_reference__create_locale = &_create_locale;

extern void slate_oracle__free_locale(_locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__free_locale), __typeof__(_free_locale)),
    "locale.h:_free_locale declaration differs from oracle");

static __typeof__(_free_locale) *const slate_reference__free_locale = &_free_locale;

extern _locale_t slate_oracle__get_current_locale(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_current_locale), __typeof__(_get_current_locale)),
    "locale.h:_get_current_locale declaration differs from oracle");

static __typeof__(_get_current_locale) *const slate_reference__get_current_locale = &_get_current_locale;

extern void slate_oracle__lock_locales(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__lock_locales), __typeof__(_lock_locales)),
    "locale.h:_lock_locales declaration differs from oracle");

static __typeof__(_lock_locales) *const slate_reference__lock_locales = &_lock_locales;

extern void slate_oracle__unlock_locales(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__unlock_locales), __typeof__(_unlock_locales)),
    "locale.h:_unlock_locales declaration differs from oracle");

static __typeof__(_unlock_locales) *const slate_reference__unlock_locales = &_unlock_locales;

extern _locale_t slate_oracle__wcreate_locale(int, const unsigned short *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wcreate_locale), __typeof__(_wcreate_locale)),
    "locale.h:_wcreate_locale declaration differs from oracle");

static __typeof__(_wcreate_locale) *const slate_reference__wcreate_locale = &_wcreate_locale;

extern unsigned short * slate_oracle__wsetlocale(int, const unsigned short *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wsetlocale), __typeof__(_wsetlocale)),
    "locale.h:_wsetlocale declaration differs from oracle");

static __typeof__(_wsetlocale) *const slate_reference__wsetlocale = &_wsetlocale;

extern struct lconv * slate_oracle_localeconv(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_localeconv), __typeof__(localeconv)),
    "locale.h:localeconv declaration differs from oracle");

static __typeof__(localeconv) *const slate_reference_localeconv = &localeconv;

extern char * slate_oracle_setlocale(int, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setlocale), __typeof__(setlocale)),
    "locale.h:setlocale declaration differs from oracle");

static __typeof__(setlocale) *const slate_reference_setlocale = &setlocale;

_Static_assert(sizeof(struct lconv) == 152, "struct lconv size differs from oracle");

_Static_assert(_Alignof(struct lconv) == 8, "struct lconv alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, decimal_point) == 0, "struct lconv.decimal_point offset differs from oracle");

typedef char * slate_oracle_struct_lconv_decimal_point;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->decimal_point), slate_oracle_struct_lconv_decimal_point), "struct lconv.decimal_point field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, thousands_sep) == 8, "struct lconv.thousands_sep offset differs from oracle");

typedef char * slate_oracle_struct_lconv_thousands_sep;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->thousands_sep), slate_oracle_struct_lconv_thousands_sep), "struct lconv.thousands_sep field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, grouping) == 16, "struct lconv.grouping offset differs from oracle");

typedef char * slate_oracle_struct_lconv_grouping;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->grouping), slate_oracle_struct_lconv_grouping), "struct lconv.grouping field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_curr_symbol) == 24, "struct lconv.int_curr_symbol offset differs from oracle");

typedef char * slate_oracle_struct_lconv_int_curr_symbol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_curr_symbol), slate_oracle_struct_lconv_int_curr_symbol), "struct lconv.int_curr_symbol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, currency_symbol) == 32, "struct lconv.currency_symbol offset differs from oracle");

typedef char * slate_oracle_struct_lconv_currency_symbol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->currency_symbol), slate_oracle_struct_lconv_currency_symbol), "struct lconv.currency_symbol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, mon_decimal_point) == 40, "struct lconv.mon_decimal_point offset differs from oracle");

typedef char * slate_oracle_struct_lconv_mon_decimal_point;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->mon_decimal_point), slate_oracle_struct_lconv_mon_decimal_point), "struct lconv.mon_decimal_point field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, mon_thousands_sep) == 48, "struct lconv.mon_thousands_sep offset differs from oracle");

typedef char * slate_oracle_struct_lconv_mon_thousands_sep;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->mon_thousands_sep), slate_oracle_struct_lconv_mon_thousands_sep), "struct lconv.mon_thousands_sep field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, mon_grouping) == 56, "struct lconv.mon_grouping offset differs from oracle");

typedef char * slate_oracle_struct_lconv_mon_grouping;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->mon_grouping), slate_oracle_struct_lconv_mon_grouping), "struct lconv.mon_grouping field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, positive_sign) == 64, "struct lconv.positive_sign offset differs from oracle");

typedef char * slate_oracle_struct_lconv_positive_sign;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->positive_sign), slate_oracle_struct_lconv_positive_sign), "struct lconv.positive_sign field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, negative_sign) == 72, "struct lconv.negative_sign offset differs from oracle");

typedef char * slate_oracle_struct_lconv_negative_sign;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->negative_sign), slate_oracle_struct_lconv_negative_sign), "struct lconv.negative_sign field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_frac_digits) == 80, "struct lconv.int_frac_digits offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_frac_digits;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_frac_digits), slate_oracle_struct_lconv_int_frac_digits), "struct lconv.int_frac_digits field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, frac_digits) == 81, "struct lconv.frac_digits offset differs from oracle");

typedef char slate_oracle_struct_lconv_frac_digits;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->frac_digits), slate_oracle_struct_lconv_frac_digits), "struct lconv.frac_digits field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, p_cs_precedes) == 82, "struct lconv.p_cs_precedes offset differs from oracle");

typedef char slate_oracle_struct_lconv_p_cs_precedes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->p_cs_precedes), slate_oracle_struct_lconv_p_cs_precedes), "struct lconv.p_cs_precedes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, p_sep_by_space) == 83, "struct lconv.p_sep_by_space offset differs from oracle");

typedef char slate_oracle_struct_lconv_p_sep_by_space;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->p_sep_by_space), slate_oracle_struct_lconv_p_sep_by_space), "struct lconv.p_sep_by_space field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, n_cs_precedes) == 84, "struct lconv.n_cs_precedes offset differs from oracle");

typedef char slate_oracle_struct_lconv_n_cs_precedes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->n_cs_precedes), slate_oracle_struct_lconv_n_cs_precedes), "struct lconv.n_cs_precedes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, n_sep_by_space) == 85, "struct lconv.n_sep_by_space offset differs from oracle");

typedef char slate_oracle_struct_lconv_n_sep_by_space;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->n_sep_by_space), slate_oracle_struct_lconv_n_sep_by_space), "struct lconv.n_sep_by_space field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, p_sign_posn) == 86, "struct lconv.p_sign_posn offset differs from oracle");

typedef char slate_oracle_struct_lconv_p_sign_posn;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->p_sign_posn), slate_oracle_struct_lconv_p_sign_posn), "struct lconv.p_sign_posn field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, n_sign_posn) == 87, "struct lconv.n_sign_posn offset differs from oracle");

typedef char slate_oracle_struct_lconv_n_sign_posn;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->n_sign_posn), slate_oracle_struct_lconv_n_sign_posn), "struct lconv.n_sign_posn field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_decimal_point) == 88, "struct lconv._W_decimal_point offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_decimal_point;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_decimal_point), slate_oracle_struct_lconv__W_decimal_point), "struct lconv._W_decimal_point field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_thousands_sep) == 96, "struct lconv._W_thousands_sep offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_thousands_sep;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_thousands_sep), slate_oracle_struct_lconv__W_thousands_sep), "struct lconv._W_thousands_sep field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_int_curr_symbol) == 104, "struct lconv._W_int_curr_symbol offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_int_curr_symbol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_int_curr_symbol), slate_oracle_struct_lconv__W_int_curr_symbol), "struct lconv._W_int_curr_symbol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_currency_symbol) == 112, "struct lconv._W_currency_symbol offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_currency_symbol;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_currency_symbol), slate_oracle_struct_lconv__W_currency_symbol), "struct lconv._W_currency_symbol field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_mon_decimal_point) == 120, "struct lconv._W_mon_decimal_point offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_mon_decimal_point;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_mon_decimal_point), slate_oracle_struct_lconv__W_mon_decimal_point), "struct lconv._W_mon_decimal_point field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_mon_thousands_sep) == 128, "struct lconv._W_mon_thousands_sep offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_mon_thousands_sep;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_mon_thousands_sep), slate_oracle_struct_lconv__W_mon_thousands_sep), "struct lconv._W_mon_thousands_sep field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_positive_sign) == 136, "struct lconv._W_positive_sign offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_positive_sign;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_positive_sign), slate_oracle_struct_lconv__W_positive_sign), "struct lconv._W_positive_sign field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, _W_negative_sign) == 144, "struct lconv._W_negative_sign offset differs from oracle");

typedef unsigned short * slate_oracle_struct_lconv__W_negative_sign;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->_W_negative_sign), slate_oracle_struct_lconv__W_negative_sign), "struct lconv._W_negative_sign field type differs from oracle");

#ifndef LC_ALL
#error "locale.h:LC_ALL macro is missing from libc-shim"
#endif

#ifndef LC_COLLATE
#error "locale.h:LC_COLLATE macro is missing from libc-shim"
#endif

#ifndef LC_CTYPE
#error "locale.h:LC_CTYPE macro is missing from libc-shim"
#endif

#ifndef LC_MAX
#error "locale.h:LC_MAX macro is missing from libc-shim"
#endif

#ifndef LC_MIN
#error "locale.h:LC_MIN macro is missing from libc-shim"
#endif

#ifndef LC_MONETARY
#error "locale.h:LC_MONETARY macro is missing from libc-shim"
#endif

#ifndef LC_NUMERIC
#error "locale.h:LC_NUMERIC macro is missing from libc-shim"
#endif

#ifndef LC_TIME
#error "locale.h:LC_TIME macro is missing from libc-shim"
#endif

#ifndef _DISABLE_PER_THREAD_LOCALE
#error "locale.h:_DISABLE_PER_THREAD_LOCALE macro is missing from libc-shim"
#endif

#ifndef _DISABLE_PER_THREAD_LOCALE_GLOBAL
#error "locale.h:_DISABLE_PER_THREAD_LOCALE_GLOBAL macro is missing from libc-shim"
#endif

#ifndef _DISABLE_PER_THREAD_LOCALE_NEW
#error "locale.h:_DISABLE_PER_THREAD_LOCALE_NEW macro is missing from libc-shim"
#endif

#ifndef _ENABLE_PER_THREAD_LOCALE
#error "locale.h:_ENABLE_PER_THREAD_LOCALE macro is missing from libc-shim"
#endif

#ifndef _ENABLE_PER_THREAD_LOCALE_GLOBAL
#error "locale.h:_ENABLE_PER_THREAD_LOCALE_GLOBAL macro is missing from libc-shim"
#endif

#ifndef _ENABLE_PER_THREAD_LOCALE_NEW
#error "locale.h:_ENABLE_PER_THREAD_LOCALE_NEW macro is missing from libc-shim"
#endif

#ifndef _INC_LOCALE
#error "locale.h:_INC_LOCALE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
