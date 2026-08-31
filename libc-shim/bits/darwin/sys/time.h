#ifndef _SLATE_BITS_DARWIN_SYS_TIME_H
#define _SLATE_BITS_DARWIN_SYS_TIME_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/sys/time.h> directly; include a public header instead."
#endif

#include <sys/select.h>

#define __NEED_struct_timeval
#include <bits/types.h>

struct itimerval {
  struct timeval it_interval;
  struct timeval it_value;
};

struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};

#define ITIMER_REAL    0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF    2

int adjtime(const struct timeval *, struct timeval *);
int futimes(int, const struct timeval *);
int lutimes(const char *, const struct timeval *);
int settimeofday(const struct timeval *, const struct timezone *);
int getitimer(int, struct itimerval *);
int gettimeofday(struct timeval *__restrict, void *__restrict);
int setitimer(int, const struct itimerval *__restrict,
              struct itimerval *__restrict);
int utimes(const char *, const struct timeval *);

#endif
