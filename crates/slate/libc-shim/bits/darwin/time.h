#ifndef _SLATE_BITS_DARWIN_TIME_H
#define _SLATE_BITS_DARWIN_TIME_H

#define __NEED_size_t
#define __NEED_time_t
#define __NEED_clock_t
#define __NEED_clockid_t
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

#define CLOCKS_PER_SEC 1000000
#define TIME_UTC       1

#define CLOCK_REALTIME             0
#define CLOCK_MONOTONIC_RAW        4
#define CLOCK_MONOTONIC_RAW_APPROX 5
#define CLOCK_MONOTONIC            6
#define CLOCK_UPTIME_RAW           8
#define CLOCK_UPTIME_RAW_APPROX    9
#define CLOCK_PROCESS_CPUTIME_ID   12
#define CLOCK_THREAD_CPUTIME_ID    16

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

struct tm *gmtime_r(const time_t *__restrict, struct tm *__restrict);
struct tm *localtime_r(const time_t *__restrict, struct tm *__restrict);
char      *asctime_r(const struct tm *__restrict, char *__restrict);
char      *ctime_r(const time_t *, char *);
char      *strptime(const char *__restrict, const char *__restrict,
                    struct tm *__restrict);
int        nanosleep(const struct timespec *, struct timespec *);
int        clock_getres(clockid_t, struct timespec *);
int        clock_gettime(clockid_t, struct timespec *);
unsigned long long clock_gettime_nsec_np(clockid_t);
int        clock_settime(clockid_t, const struct timespec *);
void       tzset(void);
time_t     timelocal(struct tm *const);
time_t     timegm(struct tm *const);

extern char *tzname[];
extern int   getdate_err;
extern long  timezone;
extern int   daylight;

#endif
