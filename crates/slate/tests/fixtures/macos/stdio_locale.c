#include <langinfo.h>
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
#include <xlocale.h>

_Static_assert(sizeof(fpos_t) == 8, "fpos_t");
_Static_assert(sizeof(mbstate_t) == 128, "mbstate_t");
_Static_assert(_Alignof(mbstate_t) == 8, "mbstate_t alignment");
_Static_assert(sizeof(wint_t) == 4, "wint_t");
_Static_assert(sizeof(wctype_t) == 4, "wctype_t");
_Static_assert(sizeof(wctrans_t) == 4, "wctrans_t");
_Static_assert(sizeof(locale_t) == 8, "locale_t");
_Static_assert(sizeof(struct __sbuf) == 16, "struct __sbuf");
_Static_assert(sizeof(struct __sFILE) == 152, "struct __sFILE");
_Static_assert(offsetof(struct __sFILE, _p) == 0, "FILE _p");
_Static_assert(offsetof(struct __sFILE, _bf) == 24, "FILE _bf");
_Static_assert(offsetof(struct __sFILE, _cookie) == 48, "FILE _cookie");
_Static_assert(offsetof(struct __sFILE, _ub) == 88, "FILE _ub");
_Static_assert(offsetof(struct __sFILE, _lb) == 120, "FILE _lb");
_Static_assert(offsetof(struct __sFILE, _offset) == 144, "FILE _offset");
_Static_assert(sizeof(struct lconv) == 96, "struct lconv");
_Static_assert(offsetof(struct lconv, int_p_cs_precedes) == 88,
               "lconv int_p_cs_precedes");
_Static_assert(offsetof(struct lconv, int_n_cs_precedes) == 89,
               "lconv int_n_cs_precedes");
_Static_assert(LC_ALL == 0, "LC_ALL");
_Static_assert(LC_COLLATE == 1, "LC_COLLATE");
_Static_assert(LC_CTYPE == 2, "LC_CTYPE");
_Static_assert(LC_MONETARY == 3, "LC_MONETARY");
_Static_assert(LC_NUMERIC == 4, "LC_NUMERIC");
_Static_assert(LC_TIME == 5, "LC_TIME");
_Static_assert(LC_MESSAGES == 6, "LC_MESSAGES");
_Static_assert(CODESET == 0, "CODESET");
_Static_assert(CRNCYSTR == 56, "CRNCYSTR");
_Static_assert(L_ctermid == 1024, "L_ctermid");
_Static_assert(sizeof(P_tmpdir) == 10, "P_tmpdir");
_Static_assert(__builtin_types_compatible_p(__typeof__(&fgetpos),
                                            int (*)(FILE *, fpos_t *)),
               "fgetpos signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&mbrtowc),
                                            size_t (*)(wchar_t *, const char *,
                                                       size_t, mbstate_t *)),
               "mbrtowc signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&newlocale),
                                            locale_t (*)(int, const char *,
                                                         locale_t)),
               "newlocale signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&freelocale),
                                            int (*)(locale_t)),
               "freelocale signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&setbuffer),
                                            void (*)(FILE *, char *, int)),
               "setbuffer signature");
_Static_assert(__builtin_types_compatible_p(__typeof__(&setlinebuf),
                                            int (*)(FILE *)),
               "setlinebuf signature");

FILE *standard_input(void) { return stdin; }

int classify_wide(wint_t value, locale_t locale) {
  return iswctype_l(value, wctype("alpha"), locale);
}

// REWRITES-MACOS-DAG: struct __sFILE
// REWRITES-MACOS-DAG: fn standard_input() -> *mut __sFILE
// REWRITES-MACOS-DAG: fn classify_wide(
