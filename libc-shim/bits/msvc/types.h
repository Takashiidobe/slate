#ifndef _SLATE_BITS_MSVC_TYPES_H
#define _SLATE_BITS_MSVC_TYPES_H

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
