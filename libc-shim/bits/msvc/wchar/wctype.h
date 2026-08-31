#ifndef _SLATE_BITS_MSVC_WCHAR_WCTYPE_H
#define _SLATE_BITS_MSVC_WCHAR_WCTYPE_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/wchar/wctype.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

#undef WEOF
#define WEOF ((wint_t)0xffff)

#define _UPPER    0x01
#define _LOWER    0x02
#define _DIGIT    0x04
#define _SPACE    0x08
#define _PUNCT    0x10
#define _CONTROL  0x20
#define _BLANK    0x40
#define _HEX      0x80
#define _LEADBYTE 0x8000
#define _ALPHA    (0x0100 | _UPPER | _LOWER)

const unsigned short *__pctype_func(void);
const wctype_t       *__pwctype_func(void);

#define _pctype  (__pctype_func())
#define _pwctype (__pwctype_func())

int iswalnum(wint_t);
int iswalpha(wint_t);
int iswascii(wint_t);
int iswblank(wint_t);
int iswcntrl(wint_t);
int iswdigit(wint_t);
int iswgraph(wint_t);
int iswlower(wint_t);
int iswprint(wint_t);
int iswpunct(wint_t);
int iswspace(wint_t);
int iswupper(wint_t);
int iswxdigit(wint_t);
int __iswcsymf(wint_t);
int __iswcsym(wint_t);

int _iswalnum_l(wint_t, _locale_t);
int _iswalpha_l(wint_t, _locale_t);
int _iswblank_l(wint_t, _locale_t);
int _iswcntrl_l(wint_t, _locale_t);
int _iswdigit_l(wint_t, _locale_t);
int _iswgraph_l(wint_t, _locale_t);
int _iswlower_l(wint_t, _locale_t);
int _iswprint_l(wint_t, _locale_t);
int _iswpunct_l(wint_t, _locale_t);
int _iswspace_l(wint_t, _locale_t);
int _iswupper_l(wint_t, _locale_t);
int _iswxdigit_l(wint_t, _locale_t);
int _iswcsymf_l(wint_t, _locale_t);
int _iswcsym_l(wint_t, _locale_t);

wint_t towupper(wint_t);
wint_t towlower(wint_t);
int    iswctype(wint_t, wctype_t);
wint_t _towupper_l(wint_t, _locale_t);
wint_t _towlower_l(wint_t, _locale_t);
int    _iswctype_l(wint_t, wctype_t, _locale_t);

#endif
