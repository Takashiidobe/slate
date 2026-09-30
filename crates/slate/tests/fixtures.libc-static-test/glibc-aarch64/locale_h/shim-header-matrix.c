#include <locale.h>

extern struct __locale_struct * slate_oracle_newlocale(int, const char *, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_newlocale), __typeof__(newlocale)),
    "locale.h:newlocale declaration differs from oracle");

static __typeof__(newlocale) *const slate_reference_newlocale = &newlocale;

_Static_assert(sizeof(struct lconv) == 96, "struct lconv size differs from oracle");

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

_Static_assert(__builtin_offsetof(struct lconv, int_p_cs_precedes) == 88, "struct lconv.int_p_cs_precedes offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_p_cs_precedes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_p_cs_precedes), slate_oracle_struct_lconv_int_p_cs_precedes), "struct lconv.int_p_cs_precedes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_p_sep_by_space) == 89, "struct lconv.int_p_sep_by_space offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_p_sep_by_space;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_p_sep_by_space), slate_oracle_struct_lconv_int_p_sep_by_space), "struct lconv.int_p_sep_by_space field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_n_cs_precedes) == 90, "struct lconv.int_n_cs_precedes offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_n_cs_precedes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_n_cs_precedes), slate_oracle_struct_lconv_int_n_cs_precedes), "struct lconv.int_n_cs_precedes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_n_sep_by_space) == 91, "struct lconv.int_n_sep_by_space offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_n_sep_by_space;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_n_sep_by_space), slate_oracle_struct_lconv_int_n_sep_by_space), "struct lconv.int_n_sep_by_space field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_p_sign_posn) == 92, "struct lconv.int_p_sign_posn offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_p_sign_posn;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_p_sign_posn), slate_oracle_struct_lconv_int_p_sign_posn), "struct lconv.int_p_sign_posn field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lconv, int_n_sign_posn) == 93, "struct lconv.int_n_sign_posn offset differs from oracle");

typedef char slate_oracle_struct_lconv_int_n_sign_posn;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lconv *)0)->int_n_sign_posn), slate_oracle_struct_lconv_int_n_sign_posn), "struct lconv.int_n_sign_posn field type differs from oracle");

#ifndef LC_ADDRESS
#error "locale.h:LC_ADDRESS macro is missing from libc-shim"
#endif

#ifndef LC_ADDRESS_MASK
#error "locale.h:LC_ADDRESS_MASK macro is missing from libc-shim"
#endif

#ifndef LC_ALL
#error "locale.h:LC_ALL macro is missing from libc-shim"
#endif

#ifndef LC_ALL_MASK
#error "locale.h:LC_ALL_MASK macro is missing from libc-shim"
#endif

#ifndef LC_COLLATE
#error "locale.h:LC_COLLATE macro is missing from libc-shim"
#endif

#ifndef LC_COLLATE_MASK
#error "locale.h:LC_COLLATE_MASK macro is missing from libc-shim"
#endif

#ifndef LC_CTYPE
#error "locale.h:LC_CTYPE macro is missing from libc-shim"
#endif

#ifndef LC_CTYPE_MASK
#error "locale.h:LC_CTYPE_MASK macro is missing from libc-shim"
#endif

#ifndef LC_GLOBAL_LOCALE
#error "locale.h:LC_GLOBAL_LOCALE macro is missing from libc-shim"
#endif

#ifndef LC_IDENTIFICATION
#error "locale.h:LC_IDENTIFICATION macro is missing from libc-shim"
#endif

#ifndef LC_IDENTIFICATION_MASK
#error "locale.h:LC_IDENTIFICATION_MASK macro is missing from libc-shim"
#endif

#ifndef LC_MEASUREMENT
#error "locale.h:LC_MEASUREMENT macro is missing from libc-shim"
#endif

#ifndef LC_MEASUREMENT_MASK
#error "locale.h:LC_MEASUREMENT_MASK macro is missing from libc-shim"
#endif

#ifndef LC_MESSAGES
#error "locale.h:LC_MESSAGES macro is missing from libc-shim"
#endif

#ifndef LC_MESSAGES_MASK
#error "locale.h:LC_MESSAGES_MASK macro is missing from libc-shim"
#endif

#ifndef LC_MONETARY
#error "locale.h:LC_MONETARY macro is missing from libc-shim"
#endif

#ifndef LC_MONETARY_MASK
#error "locale.h:LC_MONETARY_MASK macro is missing from libc-shim"
#endif

#ifndef LC_NAME
#error "locale.h:LC_NAME macro is missing from libc-shim"
#endif

#ifndef LC_NAME_MASK
#error "locale.h:LC_NAME_MASK macro is missing from libc-shim"
#endif

#ifndef LC_NUMERIC
#error "locale.h:LC_NUMERIC macro is missing from libc-shim"
#endif

#ifndef LC_NUMERIC_MASK
#error "locale.h:LC_NUMERIC_MASK macro is missing from libc-shim"
#endif

#ifndef LC_PAPER
#error "locale.h:LC_PAPER macro is missing from libc-shim"
#endif

#ifndef LC_PAPER_MASK
#error "locale.h:LC_PAPER_MASK macro is missing from libc-shim"
#endif

#ifndef LC_TELEPHONE
#error "locale.h:LC_TELEPHONE macro is missing from libc-shim"
#endif

#ifndef LC_TELEPHONE_MASK
#error "locale.h:LC_TELEPHONE_MASK macro is missing from libc-shim"
#endif

#ifndef LC_TIME
#error "locale.h:LC_TIME macro is missing from libc-shim"
#endif

#ifndef LC_TIME_MASK
#error "locale.h:LC_TIME_MASK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
