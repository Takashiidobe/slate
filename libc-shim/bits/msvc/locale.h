#ifndef _SLATE_BITS_MSVC_LOCALE_H
#define _SLATE_BITS_MSVC_LOCALE_H

#define __NEED_mbstate_t
#define __NEED_msvc_locale_layouts
#include <bits/msvc/types.h>
#undef __NEED_mbstate_t
#undef __NEED_msvc_locale_layouts

#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5

#define LC_MIN LC_ALL
#define LC_MAX LC_TIME

struct lconv {
  char    *decimal_point;
  char    *thousands_sep;
  char    *grouping;
  char    *int_curr_symbol;
  char    *currency_symbol;
  char    *mon_decimal_point;
  char    *mon_thousands_sep;
  char    *mon_grouping;
  char    *positive_sign;
  char    *negative_sign;
  char     int_frac_digits;
  char     frac_digits;
  char     p_cs_precedes;
  char     p_sep_by_space;
  char     n_cs_precedes;
  char     n_sep_by_space;
  char     p_sign_posn;
  char     n_sign_posn;
  wchar_t *_W_decimal_point;
  wchar_t *_W_thousands_sep;
  wchar_t *_W_int_curr_symbol;
  wchar_t *_W_currency_symbol;
  wchar_t *_W_mon_decimal_point;
  wchar_t *_W_mon_thousands_sep;
  wchar_t *_W_positive_sign;
  wchar_t *_W_negative_sign;
};

#define _ENABLE_PER_THREAD_LOCALE         0x0001
#define _DISABLE_PER_THREAD_LOCALE        0x0002
#define _ENABLE_PER_THREAD_LOCALE_GLOBAL  0x0010
#define _DISABLE_PER_THREAD_LOCALE_GLOBAL 0x0020
#define _ENABLE_PER_THREAD_LOCALE_NEW     0x0100
#define _DISABLE_PER_THREAD_LOCALE_NEW    0x0200

void _lock_locales(void);
void _unlock_locales(void);
int  _configthreadlocale(int);

char         *setlocale(int, const char *);
struct lconv *localeconv(void);

_locale_t _get_current_locale(void);
_locale_t _create_locale(int, const char *);
void      _free_locale(_locale_t);

wchar_t  *_wsetlocale(int, const wchar_t *);
_locale_t _wcreate_locale(int, const wchar_t *);

wchar_t    **___lc_locale_name_func(void);
unsigned int ___lc_codepage_func(void);
unsigned int ___lc_collate_cp_func(void);

#endif
