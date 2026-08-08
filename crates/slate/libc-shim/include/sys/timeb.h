#ifndef _SLATE_SYS_TIMEB_H
#define _SLATE_SYS_TIMEB_H

#define __NEED_time_t
#include <bits/types.h>

struct timeb {
  time_t         time;
  unsigned short millitm;
  short          timezone, dstflag;
};

int ftime(struct timeb *);

#endif
