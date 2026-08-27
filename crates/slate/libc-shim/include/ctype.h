#ifndef _SLATE_CTYPE_H
#define _SLATE_CTYPE_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __STDC_VERSION_CTYPE_H__ 202311L
#endif

#if defined(__SLATE_LIBC_MSVC)
#include <features.h>
#define __NEED_wint_t
#define __NEED_wctype_t
#include <bits/types.h>
#endif

int isalnum(int c);
int isalpha(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

#define __NEED_locale_t
#include <bits/types.h>

int isalnum_l(int, locale_t);
int isalpha_l(int, locale_t);
int isblank_l(int, locale_t);
int iscntrl_l(int, locale_t);
int isdigit_l(int, locale_t);
int isgraph_l(int, locale_t);
int islower_l(int, locale_t);
int isprint_l(int, locale_t);
int ispunct_l(int, locale_t);
int isspace_l(int, locale_t);
int isupper_l(int, locale_t);
int isxdigit_l(int, locale_t);
int tolower_l(int, locale_t);
int toupper_l(int, locale_t);

int isascii(int);
int toascii(int);
int _tolower(int);
int _toupper(int);

#endif

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/locale/ctype.h>
#include <bits/msvc/wchar/wctype.h>
#endif

#endif
