#ifndef _SLATE_BITS_MSVC_STDDEF_H
#define _SLATE_BITS_MSVC_STDDEF_H

#include <bits/msvc/types.h>

#ifndef __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 1
#endif

#if __STDC_WANT_SECURE_LIB__
typedef size_t rsize_t;
#endif

int           *_errno(void);
errno_t        _set_errno(int);
errno_t        _get_errno(int *);
unsigned long  __threadid(void);
__UINTPTR_TYPE__ __threadhandle(void);

#endif
