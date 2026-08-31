#ifndef _SLATE_BITS_MSVC_LOCALE_CTYPE_H
#define _SLATE_BITS_MSVC_LOCALE_CTYPE_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/locale/ctype.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

int _chvalidchk_l(int, int, _locale_t);
int _ischartype_l(int, int, _locale_t);
int _isctype_l(int, int, _locale_t);
int _isalnum_l(int, _locale_t);
int _isalpha_l(int, _locale_t);
int _isblank_l(int, _locale_t);
int _iscntrl_l(int, _locale_t);
int _isdigit_l(int, _locale_t);
int _isgraph_l(int, _locale_t);
int _isleadbyte_l(int, _locale_t);
int _islower_l(int, _locale_t);
int _isprint_l(int, _locale_t);
int _ispunct_l(int, _locale_t);
int _isspace_l(int, _locale_t);
int _isupper_l(int, _locale_t);
int _isxdigit_l(int, _locale_t);
int _tolower_l(int, _locale_t);
int _toupper_l(int, _locale_t);

#endif
