#ifndef _SLATE_TIME_H
#define _SLATE_TIME_H

#define __need_size_t
#define __need_NULL
#include <stddef.h>
#undef __need_size_t
#undef __need_NULL

typedef long time_t;
typedef long clock_t;

#define CLOCKS_PER_SEC ((clock_t)1000000)
#define TIME_UTC       1

struct tm {
  int         tm_sec;
  int         tm_min;
  int         tm_hour;
  int         tm_mday;
  int         tm_mon;
  int         tm_year;
  int         tm_wday;
  int         tm_yday;
  int         tm_isdst;
  long        tm_gmtoff;
  const char *tm_zone;
};

struct timespec {
  time_t tv_sec;
  long   tv_nsec;
};

clock_t    clock(void);
time_t     time(time_t *tloc);
double     difftime(time_t time1, time_t time0);
time_t     mktime(struct tm *tm);
struct tm *gmtime(const time_t *timer);
struct tm *gmtime_r(const time_t *restrict timer, struct tm *restrict result);
struct tm *localtime_r(const time_t *restrict timer,
                       struct tm *restrict result);
char      *asctime(const struct tm *tm);
size_t strftime(char *restrict s, size_t maxsize, const char *restrict format,
                const struct tm *restrict tm);
int    timespec_get(struct timespec *ts, int base);

#endif
