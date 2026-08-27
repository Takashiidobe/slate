#ifndef _SLATE_WCHAR_H
#define _SLATE_WCHAR_H

#include <features.h>

#include <stdarg.h>

#if defined(__SLATE_LIBC_MSVC)

#define __NEED_wchar_t
#define __NEED_FILE
#define __NEED_size_t
#define __NEED_wint_t
#define __NEED_NULL
#define __NEED_wctype_t
#define __NEED_mbstate_t
#define __NEED_time_t
#define __NEED_msvc_struct_tm
#include <bits/types.h>

#include <bits/msvc/wchar.h>

#else

#define __NEED_wchar_t
#define __NEED_FILE
#define __NEED_size_t
#define __NEED_wint_t
#define __NEED_NULL
#define __NEED_FILE
#define __NEED_wctype_t
#define __NEED_mbstate_t
#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_BIONIC) || defined(__SLATE_LIBC_DARWIN)
#define __NEED_locale_t
#endif
#include <bits/types.h>

struct tm;

#define WEOF ((wint_t) - 1)
#if defined(__WCHAR_MAX__)
#define WCHAR_MAX __WCHAR_MAX__
#else
#define WCHAR_MAX ((wchar_t) - 1)
#endif
#if defined(__WCHAR_MIN__)
#define WCHAR_MIN __WCHAR_MIN__
#else
#define WCHAR_MIN (-WCHAR_MAX - 1)
#endif

wint_t        btowc(int);
int           fwprintf(FILE *, const wchar_t *, ...);
int           fwscanf(FILE *, const wchar_t *, ...);
int           iswalnum(wint_t);
int           iswalpha(wint_t);
int           iswcntrl(wint_t);
int           iswdigit(wint_t);
int           iswgraph(wint_t);
int           iswlower(wint_t);
int           iswprint(wint_t);
int           iswpunct(wint_t);
int           iswspace(wint_t);
int           iswupper(wint_t);
int           iswxdigit(wint_t);
int           iswctype(wint_t, wctype_t);
wint_t        fgetwc(FILE *);
wchar_t      *fgetws(wchar_t *, int, FILE *);
wint_t        fputwc(wchar_t, FILE *);
int           fputws(const wchar_t *, FILE *);
int           fwide(FILE *, int);
wint_t        getwc(FILE *);
wint_t        getwchar(void);
int           mbsinit(const __mbstate_t *);
size_t        mbrlen(const char *, size_t, __mbstate_t *);
size_t        mbrtowc(wchar_t *, const char *, size_t, __mbstate_t *);
size_t        mbsrtowcs(wchar_t *, const char **, size_t, __mbstate_t *);
wint_t        putwc(wchar_t, FILE *);
wint_t        putwchar(wchar_t);
int           swprintf(wchar_t *, size_t, const wchar_t *, ...);
int           swscanf(const wchar_t *, const wchar_t *, ...);
wint_t        towlower(wint_t);
wint_t        towupper(wint_t);
wint_t        ungetwc(wint_t, FILE *);
int           vfwprintf(FILE *, const wchar_t *, va_list);
int           vwprintf(const wchar_t *, va_list);
int           vswprintf(wchar_t *, size_t, const wchar_t *, va_list);
int           vfwscanf(FILE *, const wchar_t *, va_list);
int           vswscanf(const wchar_t *, const wchar_t *, va_list);
int           vwscanf(const wchar_t *, va_list);
size_t        wcrtomb(char *, wchar_t, __mbstate_t *);
wchar_t      *wcscat(wchar_t *, const wchar_t *);
wchar_t      *wcschr(const wchar_t *, wchar_t);
int           wcscmp(const wchar_t *, const wchar_t *);
int           wcscoll(const wchar_t *, const wchar_t *);
wchar_t      *wcscpy(wchar_t *, const wchar_t *);
size_t        wcscspn(const wchar_t *, const wchar_t *);
size_t        wcsftime(wchar_t *, size_t, const wchar_t *, const struct tm *);
size_t        wcslen(const wchar_t *);
wchar_t      *wcsncat(wchar_t *, const wchar_t *, size_t);
int           wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t      *wcsncpy(wchar_t *, const wchar_t *, size_t);
wchar_t      *wcspbrk(const wchar_t *, const wchar_t *);
wchar_t      *wcsrchr(const wchar_t *, wchar_t);
size_t        wcsrtombs(char *, const wchar_t **, size_t, __mbstate_t *);
size_t        wcsspn(const wchar_t *, const wchar_t *);
wchar_t      *wcsstr(const wchar_t *, const wchar_t *);
double        wcstod(const wchar_t *, wchar_t **);
float         wcstof(const wchar_t *, wchar_t **);
long double   wcstold(const wchar_t *, wchar_t **);
wchar_t      *wcstok(wchar_t *, const wchar_t *, wchar_t **);
long int      wcstol(const wchar_t *, wchar_t **, int);
unsigned long wcstoul(const wchar_t *, wchar_t **, int);
long long     wcstoll(const wchar_t *, wchar_t **, int);
unsigned long long wcstoull(const wchar_t *, wchar_t **, int);
wchar_t           *wcswcs(const wchar_t *, const wchar_t *);
int                wcswidth(const wchar_t *, size_t);
size_t             wcsxfrm(wchar_t *, const wchar_t *, size_t);
int                wctob(wint_t);
wctype_t           wctype(const char *);
int                wcwidth(wchar_t);
wchar_t           *wmemchr(const wchar_t *, wchar_t, size_t);
int                wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t           *wmemcpy(wchar_t *, const wchar_t *, size_t);
wchar_t           *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t           *wmemset(wchar_t *, wchar_t, size_t);
int                wprintf(const wchar_t *, ...);
int                wscanf(const wchar_t *, ...);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define wcschr(s, c)                                                          \
  _Generic((s),                                                               \
      const wchar_t *: (const wchar_t *)(wcschr)((s), (c)),                  \
      default: (wcschr)((s), (c)))
#define wcsrchr(s, c)                                                         \
  _Generic((s),                                                               \
      const wchar_t *: (const wchar_t *)(wcsrchr)((s), (c)),                 \
      default: (wcsrchr)((s), (c)))
#define wcspbrk(s1, s2)                                                       \
  _Generic((s1),                                                              \
      const wchar_t *: (const wchar_t *)(wcspbrk)((s1), (s2)),               \
      default: (wcspbrk)((s1), (s2)))
#define wcsstr(haystack, needle)                                              \
  _Generic((haystack),                                                        \
      const wchar_t *: (const wchar_t *)(wcsstr)((haystack), (needle)),      \
      default: (wcsstr)((haystack), (needle)))
#define wmemchr(s, c, n)                                                      \
  _Generic((s),                                                               \
      const wchar_t *: (const wchar_t *)(wmemchr)((s), (c), (n)),            \
      default: (wmemchr)((s), (c), (n)))
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_BIONIC) || defined(__SLATE_LIBC_DARWIN)
size_t   mbsnrtowcs(wchar_t *, const char **, size_t, size_t, mbstate_t *);
FILE    *open_wmemstream(wchar_t **, size_t *);
size_t   wcsnrtombs(char *, const wchar_t **, size_t, size_t, mbstate_t *);
wchar_t *wcpcpy(wchar_t *, const wchar_t *);
wchar_t *wcpncpy(wchar_t *, const wchar_t *, size_t);
int      wcscasecmp(const wchar_t *, const wchar_t *);
int      wcscasecmp_l(const wchar_t *, const wchar_t *, locale_t);
int      wcscoll_l(const wchar_t *, const wchar_t *, locale_t);
wchar_t *wcsdup(const wchar_t *);
int      wcsncasecmp(const wchar_t *, const wchar_t *, size_t);
int      wcsncasecmp_l(const wchar_t *, const wchar_t *, size_t, locale_t);
size_t   wcsnlen(const wchar_t *, size_t);
size_t   wcsxfrm_l(wchar_t *, const wchar_t *, size_t, locale_t);
#endif

#endif

#endif
