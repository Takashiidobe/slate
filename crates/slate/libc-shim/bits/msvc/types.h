#ifndef _SLATE_BITS_MSVC_TYPES_H
#define _SLATE_BITS_MSVC_TYPES_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/types.h> directly; include a public header instead."
#endif

#ifndef __DEFINED_errno_t
typedef int errno_t;
#define __DEFINED_errno_t
#endif

#ifndef __DEFINED_wint_t
typedef unsigned short wint_t;
#define __DEFINED_wint_t
#endif

#ifndef __DEFINED_wctype_t
typedef unsigned short wctype_t;
#define __DEFINED_wctype_t
#endif

struct __crt_locale_data;
struct __crt_multibyte_data;
struct __crt_locale_pointers;

typedef struct __crt_locale_pointers *_locale_t;

#ifndef __DEFINED_msvc_time_types
typedef long      __time32_t;
typedef long long __time64_t;
#define __DEFINED_msvc_time_types
#endif

#ifndef __DEFINED_msvc_stat_types
typedef unsigned short _ino_t;
typedef unsigned int   _dev_t;
typedef long           _off_t;
#define __DEFINED_msvc_stat_types
#endif

#endif

#if defined(__NEED_msvc_locale_layouts) &&                                     \
    !defined(__DEFINED_msvc_locale_layouts)
typedef struct __crt_locale_data_public {
  const unsigned short *_locale_pctype;
  int                   _locale_mb_cur_max;
  unsigned int          _locale_lc_codepage;
} __crt_locale_data_public;

typedef struct __crt_locale_pointers {
  struct __crt_locale_data    *locinfo;
  struct __crt_multibyte_data *mbcinfo;
} __crt_locale_pointers;
#define __DEFINED_msvc_locale_layouts
#endif

#if defined(__NEED_mbstate_t) && !defined(__DEFINED_mbstate_t)
typedef struct _Mbstatet {
  unsigned long  _Wchar;
  unsigned short _Byte;
  unsigned short _State;
} _Mbstatet;
typedef _Mbstatet mbstate_t;
#define __DEFINED_mbstate_t
#endif

#if defined(__NEED_FILE) && !defined(__DEFINED_FILE)
typedef struct _iobuf FILE;
#define __DEFINED_FILE
#endif

#if defined(__NEED_wctrans_t) && !defined(__DEFINED_wctrans_t)
typedef __slate_wchar_t wctrans_t;
#define __DEFINED_wctrans_t
#endif

#if defined(__NEED_msvc_struct_tm) && !defined(__DEFINED_msvc_struct_tm)
struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
#define __DEFINED_msvc_struct_tm
#endif
#undef __NEED_msvc_struct_tm
