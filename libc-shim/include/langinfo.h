#ifndef _SLATE_LANGINFO_H
#define _SLATE_LANGINFO_H

#include <features.h>
#include <nl_types.h>

#define __NEED_locale_t

#include <bits/types.h>

#if defined(__SLATE_LIBC_DARWIN)

#define CODESET 0
#define D_T_FMT 1
#define D_FMT 2
#define T_FMT 3
#define T_FMT_AMPM 4
#define AM_STR 5
#define PM_STR 6
#define DAY_1 7
#define DAY_2 8
#define DAY_3 9
#define DAY_4 10
#define DAY_5 11
#define DAY_6 12
#define DAY_7 13
#define ABDAY_1 14
#define ABDAY_2 15
#define ABDAY_3 16
#define ABDAY_4 17
#define ABDAY_5 18
#define ABDAY_6 19
#define ABDAY_7 20
#define MON_1 21
#define MON_2 22
#define MON_3 23
#define MON_4 24
#define MON_5 25
#define MON_6 26
#define MON_7 27
#define MON_8 28
#define MON_9 29
#define MON_10 30
#define MON_11 31
#define MON_12 32
#define ABMON_1 33
#define ABMON_2 34
#define ABMON_3 35
#define ABMON_4 36
#define ABMON_5 37
#define ABMON_6 38
#define ABMON_7 39
#define ABMON_8 40
#define ABMON_9 41
#define ABMON_10 42
#define ABMON_11 43
#define ABMON_12 44
#define ERA 45
#define ERA_D_FMT 46
#define ERA_D_T_FMT 47
#define ERA_T_FMT 48
#define ALT_DIGITS 49
#define RADIXCHAR 50
#define THOUSEP 51
#define YESEXPR 52
#define NOEXPR 53
#if (__DARWIN_C_LEVEL > __DARWIN_C_ANSI && __DARWIN_C_LEVEL < 200112L) ||     \
    __DARWIN_C_LEVEL == __DARWIN_C_FULL
#define YESSTR 54
#define NOSTR 55
#endif
#define CRNCYSTR 56
#if !defined(_ANSI_SOURCE) &&                                                  \
    (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
#define D_MD_ORDER 57
#endif

#elif defined(__SLATE_LIBC_BIONIC)

#define CODESET 1
#define D_T_FMT 2
#define D_FMT 3
#define T_FMT 4
#define T_FMT_AMPM 5
#define AM_STR 6
#define PM_STR 7
#define DAY_1 8
#define DAY_2 9
#define DAY_3 10
#define DAY_4 11
#define DAY_5 12
#define DAY_6 13
#define DAY_7 14
#define ABDAY_1 15
#define ABDAY_2 16
#define ABDAY_3 17
#define ABDAY_4 18
#define ABDAY_5 19
#define ABDAY_6 20
#define ABDAY_7 21
#define MON_1 22
#define MON_2 23
#define MON_3 24
#define MON_4 25
#define MON_5 26
#define MON_6 27
#define MON_7 28
#define MON_8 29
#define MON_9 30
#define MON_10 31
#define MON_11 32
#define MON_12 33
#define ABMON_1 34
#define ABMON_2 35
#define ABMON_3 36
#define ABMON_4 37
#define ABMON_5 38
#define ABMON_6 39
#define ABMON_7 40
#define ABMON_8 41
#define ABMON_9 42
#define ABMON_10 43
#define ABMON_11 44
#define ABMON_12 45
#define ERA 46
#define ERA_D_FMT 47
#define ERA_D_T_FMT 48
#define ERA_T_FMT 49
#define ALT_DIGITS 50
#define RADIXCHAR 51
#define THOUSEP 52
#define YESEXPR 53
#define NOEXPR 54
#define CRNCYSTR 55

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
