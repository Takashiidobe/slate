#ifndef _SLATE_BITS_FREEBSD_TIME_H
#define _SLATE_BITS_FREEBSD_TIME_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/time.h> directly; include a public header instead."
#endif

#define __NEED_size_t
#define __NEED_time_t
#define __NEED_clock_t
#define __NEED_clockid_t
#define __NEED_timer_t
#define __NEED_pid_t
#define __NEED_struct_timespec
#include <bits/types.h>

struct tm {
  int   tm_sec;
  int   tm_min;
  int   tm_hour;
  int   tm_mday;
  int   tm_mon;
  int   tm_year;
  int   tm_wday;
  int   tm_yday;
  int   tm_isdst;
  long  tm_gmtoff;
  char *tm_zone;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};

#define CLOCKS_PER_SEC 128
#define TIME_UTC       1

#define CLOCK_REALTIME          0
#define CLOCK_VIRTUAL           1
#define CLOCK_PROF              2
#define CLOCK_MONOTONIC         4
#define CLOCK_UPTIME            5
#define CLOCK_UPTIME_PRECISE    7
#define CLOCK_UPTIME_FAST       8
#define CLOCK_REALTIME_PRECISE  9
#define CLOCK_REALTIME_FAST     10
#define CLOCK_MONOTONIC_PRECISE 11
#define CLOCK_MONOTONIC_FAST    12
#define CLOCK_SECOND            13
#define CLOCK_THREAD_CPUTIME_ID 14
#define CLOCK_PROCESS_CPUTIME_ID 15
#define CLOCK_TAI               16

#define CLOCK_BOOTTIME         CLOCK_MONOTONIC
#define CLOCK_REALTIME_COARSE  CLOCK_REALTIME_FAST
#define CLOCK_MONOTONIC_COARSE CLOCK_MONOTONIC_FAST

#define TIMER_RELTIME 0x0
#define TIMER_ABSTIME 0x1

clock_t    clock(void);
time_t     time(time_t *);
double     difftime(time_t, time_t);
time_t     mktime(struct tm *);
size_t     strftime(char *__restrict, size_t, const char *__restrict,
                    const struct tm *__restrict);
struct tm *gmtime(const time_t *);
struct tm *localtime(const time_t *);
char      *asctime(const struct tm *);
char      *ctime(const time_t *);
int        timespec_get(struct timespec *, int);
int        timespec_getres(struct timespec *, int);

struct tm *gmtime_r(const time_t *, struct tm *);
struct tm *localtime_r(const time_t *, struct tm *);
char      *asctime_r(const struct tm *, char *);
char      *ctime_r(const time_t *, char *);
char      *strptime(const char *__restrict, const char *__restrict,
                    struct tm *__restrict);

void tzset(void);

int nanosleep(const struct timespec *, struct timespec *);
int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
int clock_getcpuclockid(pid_t, clockid_t *);

struct sigevent;
int timer_create(clockid_t, struct sigevent *__restrict, timer_t *__restrict);
int timer_delete(timer_t);
int timer_settime(timer_t, int, const struct itimerspec *__restrict,
                  struct itimerspec *__restrict);
int timer_gettime(timer_t, struct itimerspec *);
int timer_getoverrun(timer_t);

time_t timelocal(struct tm *);
time_t timegm(struct tm *);

extern char *tzname[2];
extern int   daylight;
extern long  timezone;
extern int   getdate_err;
struct tm   *getdate(const char *);

#endif
