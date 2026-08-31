#ifndef _SLATE_BITS_MSVC_WCHAR_STRING_H
#define _SLATE_BITS_MSVC_WCHAR_STRING_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/wchar/string.h> directly; include a public header instead."
#endif

wchar_t *_wcsdup(const wchar_t *);
wchar_t *wcscat(wchar_t *, const wchar_t *);
int      wcscmp(const wchar_t *, const wchar_t *);
wchar_t *wcscpy(wchar_t *, const wchar_t *);
size_t   wcscspn(const wchar_t *, const wchar_t *);
size_t   wcslen(const wchar_t *);
size_t   wcsnlen(const wchar_t *, size_t);
wchar_t *wcsncat(wchar_t *, const wchar_t *, size_t);
int      wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t *wcsncpy(wchar_t *, const wchar_t *, size_t);
wchar_t *wcspbrk(const wchar_t *, const wchar_t *);
size_t   wcsspn(const wchar_t *, const wchar_t *);
wchar_t *wcstok(wchar_t *, const wchar_t *, wchar_t **);
wchar_t *wcschr(const wchar_t *, wchar_t);
wchar_t *wcsrchr(const wchar_t *, wchar_t);
wchar_t *wcsstr(const wchar_t *, const wchar_t *);

wchar_t *_wcserror(int);
wchar_t *__wcserror(const wchar_t *);
int      _wcsicmp(const wchar_t *, const wchar_t *);
int      _wcsicmp_l(const wchar_t *, const wchar_t *, _locale_t);
int      _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
int      _wcsnicmp_l(const wchar_t *, const wchar_t *, size_t, _locale_t);
wchar_t *_wcsnset(wchar_t *, wchar_t, size_t);
wchar_t *_wcsrev(wchar_t *);
wchar_t *_wcsset(wchar_t *, wchar_t);
wchar_t *_wcslwr(wchar_t *);
wchar_t *_wcslwr_l(wchar_t *, _locale_t);
wchar_t *_wcsupr(wchar_t *);
wchar_t *_wcsupr_l(wchar_t *, _locale_t);
size_t   wcsxfrm(wchar_t *, const wchar_t *, size_t);
size_t   _wcsxfrm_l(wchar_t *, const wchar_t *, size_t, _locale_t);
int      wcscoll(const wchar_t *, const wchar_t *);
int      _wcscoll_l(const wchar_t *, const wchar_t *, _locale_t);
int      _wcsicoll(const wchar_t *, const wchar_t *);
int      _wcsicoll_l(const wchar_t *, const wchar_t *, _locale_t);
int      _wcsncoll(const wchar_t *, const wchar_t *, size_t);
int      _wcsncoll_l(const wchar_t *, const wchar_t *, size_t, _locale_t);
int      _wcsnicoll(const wchar_t *, const wchar_t *, size_t);
int      _wcsnicoll_l(const wchar_t *, const wchar_t *, size_t, _locale_t);

#endif
