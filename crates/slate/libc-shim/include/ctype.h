#ifndef _SLATE_CTYPE_H
#define _SLATE_CTYPE_H

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

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/locale/ctype.h>
#include <bits/msvc/wchar/wctype.h>
#endif

#endif
