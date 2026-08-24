#ifndef _SLATE_LOCALE_H
#define _SLATE_LOCALE_H

#include <features.h>

#if defined(__SLATE_LIBC_MSVC)

#define __NEED_wchar_t
#include <bits/types.h>

#include <bits/msvc/locale.h>

#else

#if defined(__SLATE_LIBC_DARWIN)
#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5
#define LC_MESSAGES 6
#else
#define LC_CTYPE    0
#define LC_NUMERIC  1
#define LC_TIME     2
#define LC_COLLATE  3
#define LC_MONETARY 4
#define LC_MESSAGES 5
#define LC_ALL      6
#endif
#if !defined(__SLATE_LIBC_DARWIN)
#define LC_PAPER          7
#define LC_NAME           8
#define LC_ADDRESS        9
#define LC_TELEPHONE      10
#define LC_MEASUREMENT    11
#define LC_IDENTIFICATION 12
#endif

/* Field order/types must match the real glibc struct lconv exactly:
   localeconv() returns a pointer into glibc's own static instance. */
struct lconv {
  char *decimal_point;
  char *thousands_sep;
  char *grouping;

  char *int_curr_symbol;
  char *currency_symbol;
  char *mon_decimal_point;
  char *mon_thousands_sep;
  char *mon_grouping;
  char *positive_sign;
  char *negative_sign;
  char  int_frac_digits;
  char  frac_digits;
  char  p_cs_precedes;
  char  p_sep_by_space;
  char  n_cs_precedes;
  char  n_sep_by_space;
  char  p_sign_posn;
  char  n_sign_posn;
#if defined(__SLATE_LIBC_DARWIN)
  char  int_p_cs_precedes;
  char  int_n_cs_precedes;
  char  int_p_sep_by_space;
#else
  char  int_p_cs_precedes;
  char  int_p_sep_by_space;
  char  int_n_cs_precedes;
#endif
  char  int_n_sep_by_space;
  char  int_p_sign_posn;
  char  int_n_sign_posn;
};

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||  \
    defined(__SLATE_LIBC_BIONIC) || defined(__SLATE_LIBC_DARWIN)
#define __NEED_NULL
#define __NEED_locale_t
#include <bits/types.h>

#define LC_GLOBAL_LOCALE ((locale_t) - 1)

#if defined(__SLATE_LIBC_DARWIN)
#define LC_COLLATE_MASK  (1 << 0)
#define LC_CTYPE_MASK    (1 << 1)
#define LC_MESSAGES_MASK (1 << 2)
#define LC_MONETARY_MASK (1 << 3)
#define LC_NUMERIC_MASK  (1 << 4)
#define LC_TIME_MASK     (1 << 5)
#define LC_ALL_MASK                                                           \
  (LC_COLLATE_MASK | LC_CTYPE_MASK | LC_MESSAGES_MASK | LC_MONETARY_MASK |   \
   LC_NUMERIC_MASK | LC_TIME_MASK)
#else
#define LC_CTYPE_MASK          (1 << LC_CTYPE)
#define LC_NUMERIC_MASK        (1 << LC_NUMERIC)
#define LC_TIME_MASK           (1 << LC_TIME)
#define LC_COLLATE_MASK        (1 << LC_COLLATE)
#define LC_MONETARY_MASK       (1 << LC_MONETARY)
#define LC_MESSAGES_MASK       (1 << LC_MESSAGES)
#define LC_PAPER_MASK          (1 << LC_PAPER)
#define LC_NAME_MASK           (1 << LC_NAME)
#define LC_ADDRESS_MASK        (1 << LC_ADDRESS)
#define LC_TELEPHONE_MASK      (1 << LC_TELEPHONE)
#define LC_MEASUREMENT_MASK    (1 << LC_MEASUREMENT)
#define LC_IDENTIFICATION_MASK (1 << LC_IDENTIFICATION)
#define LC_ALL_MASK      0x7fffffff
#endif

locale_t duplocale(locale_t);
#if defined(__SLATE_LIBC_DARWIN)
int      freelocale(locale_t);
#else
void     freelocale(locale_t);
#endif
locale_t newlocale(int, const char *, locale_t);
locale_t uselocale(locale_t);
#endif

char         *setlocale(int category, const char *locale);
struct lconv *localeconv(void);

#endif

#endif
