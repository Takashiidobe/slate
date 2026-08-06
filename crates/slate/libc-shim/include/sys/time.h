#ifndef _SLATE_SYS_TIME_H
#define _SLATE_SYS_TIME_H

#include <features.h>

#include <sys/select.h>

#define __NEED_struct_timeval
#include <bits/types.h>

int gettimeofday(struct timeval *__restrict, void *__restrict);

enum {
  ITIMER_REAL    = 0,
  ITIMER_VIRTUAL = 1,
  ITIMER_PROF    = 2,
};

struct itimerval {
  struct timeval it_interval;
  struct timeval it_value;
};

int getitimer(int, struct itimerval *);
int setitimer(int, const struct itimerval *__restrict,
              struct itimerval *__restrict);
int utimes(const char *, const struct timeval[2]);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
int futimes(int, const struct timeval[2]);
int futimesat(int, const char *, const struct timeval[2]);
int lutimes(const char *, const struct timeval[2]);
int settimeofday(const struct timeval *, const struct timezone *);
int adjtime(const struct timeval *, struct timeval *);
#include <stdbool.h>
#include <sys/time.h>

bool timerisset(const struct timeval *tv);
void timerclear(struct timeval *tv);
int  timercmp(const struct timeval *a, const struct timeval *b);
void timeradd(const struct timeval *a, const struct timeval *b,
              struct timeval *res);
void timersub(const struct timeval *a, const struct timeval *b,
              struct timeval *res);
#endif

#if defined(_GNU_SOURCE)
void timeval_to_timespec(const struct timeval *tv, struct timespec *ts);
void timespec_to_timeval(struct timeval *tv, const struct timespec *ts);
#endif

#if _REDIR_TIME64
__REDIR(gettimeofday, __gettimeofday_time64);
__REDIR(getitimer, __getitimer_time64);
__REDIR(setitimer, __setitimer_time64);
__REDIR(utimes, __utimes_time64);
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
__REDIR(futimes, __futimes_time64);
__REDIR(futimesat, __futimesat_time64);
__REDIR(lutimes, __lutimes_time64);
__REDIR(settimeofday, __settimeofday_time64);
__REDIR(adjtime, __adjtime64);
#endif
#endif

#endif
