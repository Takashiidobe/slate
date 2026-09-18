#ifndef _SLATE_SYS_TIME_H
#define _SLATE_SYS_TIME_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/sys/time.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/sys/time.h>

#else

#include <sys/select.h>

#define ITIMER_REAL 0
#define ITIMER_VIRTUAL 1
#define ITIMER_PROF 2

#define TIMESPEC_TO_TIMEVAL(tv, ts) \
  do { \
    (tv)->tv_sec = (ts)->tv_sec; \
    (tv)->tv_usec = (ts)->tv_nsec / 1000; \
  } while (0)
#define TIMEVAL_TO_TIMESPEC(tv, ts) \
  do { \
    (ts)->tv_sec = (tv)->tv_sec; \
    (ts)->tv_nsec = (tv)->tv_usec * 1000; \
  } while (0)
#define timerisset(tv) ((tv)->tv_sec || (tv)->tv_usec)
#define timerclear(tv) ((tv)->tv_sec = (tv)->tv_usec = 0)
#define timercmp(a, b, op) ((a)->tv_sec op (b)->tv_sec || \
                            ((a)->tv_sec == (b)->tv_sec && \
                             (a)->tv_usec op (b)->tv_usec))
#define timeradd(a, b, res) \
  do { \
    (res)->tv_sec = (a)->tv_sec + (b)->tv_sec; \
    (res)->tv_usec = (a)->tv_usec + (b)->tv_usec; \
    if ((res)->tv_usec >= 1000000) { \
      ++(res)->tv_sec; \
      (res)->tv_usec -= 1000000; \
    } \
  } while (0)
#define timersub(a, b, res) \
  do { \
    (res)->tv_sec = (a)->tv_sec - (b)->tv_sec; \
    (res)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
    if ((res)->tv_usec < 0) { \
      --(res)->tv_sec; \
      (res)->tv_usec += 1000000; \
    } \
  } while (0)

#define __NEED_struct_timeval
#include <bits/types.h>

int gettimeofday(struct timeval *__restrict, void *__restrict);

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

#endif
