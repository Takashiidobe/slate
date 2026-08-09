#ifndef _SLATE_BITS_MSVC_WCTYPE_H
#define _SLATE_BITS_MSVC_WCTYPE_H

#include <bits/msvc/wchar/wctype.h>

wint_t    towctrans(wint_t, wctrans_t);
wctrans_t wctrans(const char *);
wctype_t  wctype(const char *);

#endif
