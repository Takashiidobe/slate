#ifndef _SLATE_LANGINFO_H
#define _SLATE_LANGINFO_H

#include <features.h>
#include <nl_types.h>

#define __NEED_locale_t

#include <bits/types.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/langinfo.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/langinfo.h>

#elif defined(__SLATE_LIBC_BIONIC)

#include <bits/bionic/langinfo.h>

#else

#define ABDAY_1 0x20000
#define ABDAY_2 0x20001
#define ABDAY_3 0x20002
#define ABDAY_4 0x20003
#define ABDAY_5 0x20004
#define ABDAY_6 0x20005
#define ABDAY_7 0x20006

#define DAY_1 0x20007
#define DAY_2 0x20008
#define DAY_3 0x20009
#define DAY_4 0x2000A
#define DAY_5 0x2000B
#define DAY_6 0x2000C
#define DAY_7 0x2000D

#define ABMON_1  0x2000E
#define ABMON_2  0x2000F
#define ABMON_3  0x20010
#define ABMON_4  0x20011
#define ABMON_5  0x20012
#define ABMON_6  0x20013
#define ABMON_7  0x20014
#define ABMON_8  0x20015
#define ABMON_9  0x20016
#define ABMON_10 0x20017
#define ABMON_11 0x20018
#define ABMON_12 0x20019

#define MON_1  0x2001A
#define MON_2  0x2001B
#define MON_3  0x2001C
#define MON_4  0x2001D
#define MON_5  0x2001E
#define MON_6  0x2001F
#define MON_7  0x20020
#define MON_8  0x20021
#define MON_9  0x20022
#define MON_10 0x20023
#define MON_11 0x20024
#define MON_12 0x20025

#define AM_STR 0x20026
#define PM_STR 0x20027

#define D_T_FMT    0x20028
#define D_FMT      0x20029
#define T_FMT      0x2002A
#define T_FMT_AMPM 0x2002B

#define ERA         0x2002C
#if defined(__SLATE_LIBC_GLIBC)
#define ERA_YEAR __ERA_YEAR
#define ALTMON_1 __ALTMON_1
#define ALTMON_2 __ALTMON_2
#define ALTMON_3 __ALTMON_3
#define ALTMON_4 __ALTMON_4
#define ALTMON_5 __ALTMON_5
#define ALTMON_6 __ALTMON_6
#define ALTMON_7 __ALTMON_7
#define ALTMON_8 __ALTMON_8
#define ALTMON_9 __ALTMON_9
#define ALTMON_10 __ALTMON_10
#define ALTMON_11 __ALTMON_11
#define ALTMON_12 __ALTMON_12
#define INT_CURR_SYMBOL __INT_CURR_SYMBOL
#define CURRENCY_SYMBOL __CURRENCY_SYMBOL
#define MON_DECIMAL_POINT __MON_DECIMAL_POINT
#define MON_THOUSANDS_SEP __MON_THOUSANDS_SEP
#define MON_GROUPING __MON_GROUPING
#define POSITIVE_SIGN __POSITIVE_SIGN
#define NEGATIVE_SIGN __NEGATIVE_SIGN
#define INT_FRAC_DIGITS __INT_FRAC_DIGITS
#define FRAC_DIGITS __FRAC_DIGITS
#define P_CS_PRECEDES __P_CS_PRECEDES
#define P_SEP_BY_SPACE __P_SEP_BY_SPACE
#define N_CS_PRECEDES __N_CS_PRECEDES
#define N_SEP_BY_SPACE __N_SEP_BY_SPACE
#define P_SIGN_POSN __P_SIGN_POSN
#define N_SIGN_POSN __N_SIGN_POSN
#define INT_P_CS_PRECEDES __INT_P_CS_PRECEDES
#define INT_P_SEP_BY_SPACE __INT_P_SEP_BY_SPACE
#define INT_N_CS_PRECEDES __INT_N_CS_PRECEDES
#define INT_N_SEP_BY_SPACE __INT_N_SEP_BY_SPACE
#define INT_P_SIGN_POSN __INT_P_SIGN_POSN
#define INT_N_SIGN_POSN __INT_N_SIGN_POSN
#define DECIMAL_POINT __DECIMAL_POINT
#define THOUSANDS_SEP __THOUSANDS_SEP
#define GROUPING __GROUPING
#endif
#define ERA_D_FMT   0x2002E
#define ALT_DIGITS  0x2002F
#define ERA_D_T_FMT 0x20030
#define ERA_T_FMT   0x20031

#define CODESET 14

#define CRNCYSTR 0x4000F

#define RADIXCHAR 0x10000
#define THOUSEP   0x10001
#define YESEXPR   0x50000
#define NOEXPR    0x50001

#define _NL_LOCALE_NAME(cat) (((cat) << 16) | 0xffff)

#if defined(_GNU_SOURCE)
#define NL_LOCALE_NAME(cat) _NL_LOCALE_NAME(cat)
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define YESSTR 0x50002
#define NOSTR  0x50003
#endif

#endif

char *nl_langinfo(nl_item);
char *nl_langinfo_l(nl_item, locale_t);

#endif
