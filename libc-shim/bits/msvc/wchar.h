#ifndef _SLATE_BITS_MSVC_WCHAR_H
#define _SLATE_BITS_MSVC_WCHAR_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/wchar.h> directly; include a public header instead."
#endif

#define WCHAR_MIN 0x0000
#define WCHAR_MAX 0xffff
#define WEOF      ((wint_t)0xffff)

typedef wchar_t _Wint_t;

wchar_t  *_wsetlocale(int, const wchar_t *);
_locale_t _wcreate_locale(int, const wchar_t *);

wint_t btowc(int);
size_t mbrlen(const char *, size_t, mbstate_t *);
size_t mbrtowc(wchar_t *, const char *, size_t, mbstate_t *);
size_t mbsrtowcs(wchar_t *, const char **, size_t, mbstate_t *);
size_t wcrtomb(char *, wchar_t, mbstate_t *);
size_t wcsrtombs(char *, const wchar_t **, size_t, mbstate_t *);
int    wctob(wint_t);

int fwide(FILE *, int);
int mbsinit(const mbstate_t *);

wchar_t *wmemchr(const wchar_t *, wchar_t, size_t);
int      wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wmemcpy(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t *wmemset(wchar_t *, wchar_t, size_t);

#include <bits/msvc/wchar/stdio.h>
#include <bits/msvc/wchar/stdlib.h>
#include <bits/msvc/wchar/string.h>
#include <bits/msvc/wchar/wctype.h>
#include <bits/msvc/secure/wchar.h>
#include <bits/msvc/process.h>

#endif
