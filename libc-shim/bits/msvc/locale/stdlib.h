#ifndef _SLATE_BITS_MSVC_LOCALE_STDLIB_H
#define _SLATE_BITS_MSVC_LOCALE_STDLIB_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/locale/stdlib.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

int ___mb_cur_max_func(void);
int ___mb_cur_max_l_func(_locale_t);

#define MB_CUR_MAX   (___mb_cur_max_func())
#define __mb_cur_max (___mb_cur_max_func())

int    mblen(const char *, size_t);
int    _mblen_l(const char *, size_t, _locale_t);
size_t _mbstrlen(const char *);
size_t _mbstrlen_l(const char *, _locale_t);
size_t _mbstrnlen(const char *, size_t);
size_t _mbstrnlen_l(const char *, size_t, _locale_t);
int    mbtowc(wchar_t *, const char *, size_t);
int    _mbtowc_l(wchar_t *, const char *, size_t, _locale_t);
size_t _mbstowcs_l(wchar_t *, const char *, size_t, _locale_t);
int    wctomb(char *, wchar_t);
int    _wctomb_l(char *, wchar_t, _locale_t);
size_t _wcstombs_l(char *, const wchar_t *, size_t, _locale_t);

double             _atof_l(const char *, _locale_t);
int                _atoi_l(const char *, _locale_t);
long               _atol_l(const char *, _locale_t);
long long          _atoll_l(const char *, _locale_t);
long long          _atoi64_l(const char *, _locale_t);
double             _strtod_l(const char *, char **, _locale_t);
float              _strtof_l(const char *, char **, _locale_t);
long double        _strtold_l(const char *, char **, _locale_t);
long               _strtol_l(const char *, char **, int, _locale_t);
unsigned long      _strtoul_l(const char *, char **, int, _locale_t);
long long          _strtoll_l(const char *, char **, int, _locale_t);
unsigned long long _strtoull_l(const char *, char **, int, _locale_t);
long long          _strtoi64_l(const char *, char **, int, _locale_t);
unsigned long long _strtoui64_l(const char *, char **, int, _locale_t);

#endif
