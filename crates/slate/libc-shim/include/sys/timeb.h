#ifndef _SLATE_SYS_TIMEB_H
#define _SLATE_SYS_TIMEB_H

#include <features.h>

#if defined(__SLATE_LIBC_MSVC)

#define __NEED_time_t
#include <bits/types.h>

#include <bits/msvc/sys/timeb.h>

#else

#define __NEED_time_t
#include <bits/types.h>

struct timeb {
  time_t         time;
  unsigned short millitm;
  short          timezone, dstflag;
};

int ftime(struct timeb *);

#endif

#endif
