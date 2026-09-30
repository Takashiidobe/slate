#include <wchar.h>

extern _locale_t slate_oracle__wcreate_locale(int, const unsigned short *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wcreate_locale), __typeof__(_wcreate_locale)),
    "wchar.h:_wcreate_locale declaration differs from oracle");

static __typeof__(_wcreate_locale) *const slate_reference__wcreate_locale = &_wcreate_locale;

extern unsigned short * slate_oracle__wsetlocale(int, const unsigned short *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wsetlocale), __typeof__(_wsetlocale)),
    "wchar.h:_wsetlocale declaration differs from oracle");

static __typeof__(_wsetlocale) *const slate_reference__wsetlocale = &_wsetlocale;

extern unsigned short slate_oracle_btowc(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_btowc), __typeof__(btowc)),
    "wchar.h:btowc declaration differs from oracle");

static __typeof__(btowc) *const slate_reference_btowc = &btowc;

extern int slate_oracle_fwide(struct _iobuf *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fwide), __typeof__(fwide)),
    "wchar.h:fwide declaration differs from oracle");

static __typeof__(fwide) *const slate_reference_fwide = &fwide;

extern unsigned long long slate_oracle_mbrlen(const char *, unsigned long long, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbrlen), __typeof__(mbrlen)),
    "wchar.h:mbrlen declaration differs from oracle");

static __typeof__(mbrlen) *const slate_reference_mbrlen = &mbrlen;

extern unsigned long long slate_oracle_mbrtowc(unsigned short *, const char *, unsigned long long, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbrtowc), __typeof__(mbrtowc)),
    "wchar.h:mbrtowc declaration differs from oracle");

static __typeof__(mbrtowc) *const slate_reference_mbrtowc = &mbrtowc;

extern int slate_oracle_mbsinit(const struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbsinit), __typeof__(mbsinit)),
    "wchar.h:mbsinit declaration differs from oracle");

static __typeof__(mbsinit) *const slate_reference_mbsinit = &mbsinit;

extern unsigned long long slate_oracle_mbsrtowcs(unsigned short *, const char **, unsigned long long, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbsrtowcs), __typeof__(mbsrtowcs)),
    "wchar.h:mbsrtowcs declaration differs from oracle");

static __typeof__(mbsrtowcs) *const slate_reference_mbsrtowcs = &mbsrtowcs;

extern int slate_oracle_mbsrtowcs_s(unsigned long long *, unsigned short *, unsigned long long, const char **, unsigned long long, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbsrtowcs_s), __typeof__(mbsrtowcs_s)),
    "wchar.h:mbsrtowcs_s declaration differs from oracle");

static __typeof__(mbsrtowcs_s) *const slate_reference_mbsrtowcs_s = &mbsrtowcs_s;

extern unsigned long long slate_oracle_wcrtomb(char *, unsigned short, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcrtomb), __typeof__(wcrtomb)),
    "wchar.h:wcrtomb declaration differs from oracle");

static __typeof__(wcrtomb) *const slate_reference_wcrtomb = &wcrtomb;

extern int slate_oracle_wcrtomb_s(unsigned long long *, char *, unsigned long long, unsigned short, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcrtomb_s), __typeof__(wcrtomb_s)),
    "wchar.h:wcrtomb_s declaration differs from oracle");

static __typeof__(wcrtomb_s) *const slate_reference_wcrtomb_s = &wcrtomb_s;

extern unsigned long long slate_oracle_wcsrtombs(char *, const unsigned short **, unsigned long long, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcsrtombs), __typeof__(wcsrtombs)),
    "wchar.h:wcsrtombs declaration differs from oracle");

static __typeof__(wcsrtombs) *const slate_reference_wcsrtombs = &wcsrtombs;

extern int slate_oracle_wcsrtombs_s(unsigned long long *, char *, unsigned long long, const unsigned short **, unsigned long long, struct _Mbstatet *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcsrtombs_s), __typeof__(wcsrtombs_s)),
    "wchar.h:wcsrtombs_s declaration differs from oracle");

static __typeof__(wcsrtombs_s) *const slate_reference_wcsrtombs_s = &wcsrtombs_s;

extern int slate_oracle_wctob(unsigned short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wctob), __typeof__(wctob)),
    "wchar.h:wctob declaration differs from oracle");

static __typeof__(wctob) *const slate_reference_wctob = &wctob;

extern unsigned short * slate_oracle_wmemchr(const unsigned short *, unsigned short, __SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemchr), __typeof__(wmemchr)),
    "wchar.h:wmemchr declaration differs from oracle");

static __typeof__(wmemchr) *const slate_reference_wmemchr = &wmemchr;

extern int slate_oracle_wmemcmp(const unsigned short *, const unsigned short *, __SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemcmp), __typeof__(wmemcmp)),
    "wchar.h:wmemcmp declaration differs from oracle");

static __typeof__(wmemcmp) *const slate_reference_wmemcmp = &wmemcmp;

extern unsigned short * slate_oracle_wmemcpy(unsigned short *, const unsigned short *, __SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemcpy), __typeof__(wmemcpy)),
    "wchar.h:wmemcpy declaration differs from oracle");

static __typeof__(wmemcpy) *const slate_reference_wmemcpy = &wmemcpy;

extern int slate_oracle_wmemcpy_s(unsigned short *, unsigned long long, const unsigned short *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemcpy_s), __typeof__(wmemcpy_s)),
    "wchar.h:wmemcpy_s declaration differs from oracle");

static __typeof__(wmemcpy_s) *const slate_reference_wmemcpy_s = &wmemcpy_s;

extern unsigned short * slate_oracle_wmemmove(unsigned short *, const unsigned short *, __SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemmove), __typeof__(wmemmove)),
    "wchar.h:wmemmove declaration differs from oracle");

static __typeof__(wmemmove) *const slate_reference_wmemmove = &wmemmove;

extern int slate_oracle_wmemmove_s(unsigned short *, unsigned long long, const unsigned short *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemmove_s), __typeof__(wmemmove_s)),
    "wchar.h:wmemmove_s declaration differs from oracle");

static __typeof__(wmemmove_s) *const slate_reference_wmemmove_s = &wmemmove_s;

extern unsigned short * slate_oracle_wmemset(unsigned short *, unsigned short, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wmemset), __typeof__(wmemset)),
    "wchar.h:wmemset declaration differs from oracle");

static __typeof__(wmemset) *const slate_reference_wmemset = &wmemset;

#ifndef WCHAR_MAX
#error "wchar.h:WCHAR_MAX macro is missing from libc-shim"
#endif

#ifndef WCHAR_MIN
#error "wchar.h:WCHAR_MIN macro is missing from libc-shim"
#endif

#ifndef _INC_WCHAR
#error "wchar.h:_INC_WCHAR macro is missing from libc-shim"
#endif

int main(void) { return 0; }
