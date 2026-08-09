#ifndef _SLATE_BITS_MSVC_WCHAR_STDLIB_H
#define _SLATE_BITS_MSVC_WCHAR_STDLIB_H

double             wcstod(const wchar_t *, wchar_t **);
double             _wcstod_l(const wchar_t *, wchar_t **, _locale_t);
float              wcstof(const wchar_t *, wchar_t **);
float              _wcstof_l(const wchar_t *, wchar_t **, _locale_t);
long double        wcstold(const wchar_t *, wchar_t **);
long double        _wcstold_l(const wchar_t *, wchar_t **, _locale_t);
long               wcstol(const wchar_t *, wchar_t **, int);
long               _wcstol_l(const wchar_t *, wchar_t **, int, _locale_t);
unsigned long      wcstoul(const wchar_t *, wchar_t **, int);
unsigned long      _wcstoul_l(const wchar_t *, wchar_t **, int, _locale_t);
long long          wcstoll(const wchar_t *, wchar_t **, int);
long long          _wcstoll_l(const wchar_t *, wchar_t **, int, _locale_t);
unsigned long long wcstoull(const wchar_t *, wchar_t **, int);
unsigned long long _wcstoull_l(const wchar_t *, wchar_t **, int, _locale_t);

double    _wtof(const wchar_t *);
double    _wtof_l(const wchar_t *, _locale_t);
int       _wtoi(const wchar_t *);
int       _wtoi_l(const wchar_t *, _locale_t);
long      _wtol(const wchar_t *);
long      _wtol_l(const wchar_t *, _locale_t);
long long _wtoll(const wchar_t *);
long long _wtoll_l(const wchar_t *, _locale_t);
long long _wtoi64(const wchar_t *);
long long _wtoi64_l(const wchar_t *, _locale_t);
long long _wcstoi64(const wchar_t *, wchar_t **, int);
long long _wcstoi64_l(const wchar_t *, wchar_t **, int, _locale_t);
unsigned long long _wcstoui64(const wchar_t *, wchar_t **, int);
unsigned long long _wcstoui64_l(const wchar_t *, wchar_t **, int, _locale_t);

wchar_t *_itow(int, wchar_t *, int);
wchar_t *_ltow(long, wchar_t *, int);
wchar_t *_ultow(unsigned long, wchar_t *, int);
wchar_t *_i64tow(long long, wchar_t *, int);
wchar_t *_ui64tow(unsigned long long, wchar_t *, int);

#endif
