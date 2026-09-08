#ifndef _SLATE_CTYPE_H
#define _SLATE_CTYPE_H

#include <features.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __STDC_VERSION_CTYPE_H__ 202311L
#endif

#if defined(__SLATE_LIBC_MSVC)
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

#define isalnum(c) (isalnum)(c)
#define isalpha(c) (isalpha)(c)
#define isblank(c) (isblank)(c)
#define iscntrl(c) (iscntrl)(c)
#define isdigit(c) (isdigit)(c)
#define isgraph(c) (isgraph)(c)
#define islower(c) (islower)(c)
#define isprint(c) (isprint)(c)
#define ispunct(c) (ispunct)(c)
#define isspace(c) (isspace)(c)
#define isupper(c) (isupper)(c)
#define isxdigit(c) (isxdigit)(c)
#define tolower(c) (tolower)(c)
#define toupper(c) (toupper)(c)

#if defined(__SLATE_LIBC_GLIBC)
int isctype(int, int);
#endif

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

#define isalnum_l(c, l) (isalnum_l)(c, l)
#define isalpha_l(c, l) (isalpha_l)(c, l)
#define isblank_l(c, l) (isblank_l)(c, l)
#define iscntrl_l(c, l) (iscntrl_l)(c, l)
#define isdigit_l(c, l) (isdigit_l)(c, l)
#define isgraph_l(c, l) (isgraph_l)(c, l)
#define islower_l(c, l) (islower_l)(c, l)
#define isprint_l(c, l) (isprint_l)(c, l)
#define ispunct_l(c, l) (ispunct_l)(c, l)
#define isspace_l(c, l) (isspace_l)(c, l)
#define isupper_l(c, l) (isupper_l)(c, l)
#define isxdigit_l(c, l) (isxdigit_l)(c, l)
#define tolower_l(c, l) (tolower_l)(c, l)
#define toupper_l(c, l) (toupper_l)(c, l)
#define isascii(c) (isascii)(c)
#define toascii(c) (toascii)(c)
#define toascii_l(c, l) ((l), toascii(c))
#if defined(__SLATE_LIBC_GLIBC)
#define isascii_l(c, l) ((l), isascii(c))
#endif

#endif

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/locale/ctype.h>
#include <bits/msvc/wchar/wctype.h>
#endif

#endif
