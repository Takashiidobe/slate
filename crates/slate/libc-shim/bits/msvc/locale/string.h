#ifndef _SLATE_BITS_MSVC_LOCALE_STRING_H
#define _SLATE_BITS_MSVC_LOCALE_STRING_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/locale/string.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

int    _memicmp_l(const void *, const void *, size_t, _locale_t);
int    _strcoll_l(const char *, const char *, _locale_t);
int    _stricmp_l(const char *, const char *, _locale_t);
int    _stricoll_l(const char *, const char *, _locale_t);
char  *_strlwr_l(char *, _locale_t);
int    _strncoll_l(const char *, const char *, size_t, _locale_t);
int    _strnicmp_l(const char *, const char *, size_t, _locale_t);
int    _strnicoll_l(const char *, const char *, size_t, _locale_t);
char  *_strupr_l(char *, _locale_t);
size_t _strxfrm_l(char *, const char *, size_t, _locale_t);

#endif
