#ifndef _SLATE_BITS_DARWIN_LANGINFO_H
#define _SLATE_BITS_DARWIN_LANGINFO_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/langinfo.h> directly; include a public header instead."
#endif

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

#endif
