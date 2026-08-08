#ifndef _SLATE_SYS_TIMES_H
#define _SLATE_SYS_TIMES_H

#define __NEED_clock_t
#include <bits/types.h>

struct tms {
  clock_t tms_utime;
  clock_t tms_stime;
  clock_t tms_cutime;
  clock_t tms_cstime;
};

clock_t times(struct tms *);

#endif
