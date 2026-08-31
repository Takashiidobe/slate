#ifndef _SLATE_BITS_MSVC_SYS_TIMEB_H
#define _SLATE_BITS_MSVC_SYS_TIMEB_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/sys/timeb.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

struct __timeb32 {
  __time32_t     time;
  unsigned short millitm;
  short          timezone;
  short          dstflag;
};

struct __timeb64 {
  __time64_t     time;
  unsigned short millitm;
  short          timezone;
  short          dstflag;
};

void _ftime32(struct __timeb32 *);
void _ftime64(struct __timeb64 *);

#endif
