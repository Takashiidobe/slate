#ifndef _SLATE_BITS_MSVC_TIME_H
#define _SLATE_BITS_MSVC_TIME_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/time.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

struct _timespec32 {
  __time32_t tv_sec;
  long       tv_nsec;
};

struct _timespec64 {
  __time64_t tv_sec;
  long       tv_nsec;
};

struct timespec {
  time_t tv_sec;
  long   tv_nsec;
};

#define CLOCKS_PER_SEC ((clock_t)1000)
#define TIME_UTC       1

int   *__daylight(void);
long  *__dstbias(void);
long  *__timezone(void);
char **__tzname(void);

errno_t _get_daylight(int *);
errno_t _get_dstbias(long *);
errno_t _get_timezone(long *);
errno_t _get_tzname(size_t *, char *, size_t, int);

char     *asctime(const struct tm *);
clock_t   clock(void);
char     *_ctime32(const __time32_t *);
char     *_ctime64(const __time64_t *);
double    _difftime32(__time32_t, __time32_t);
double    _difftime64(__time64_t, __time64_t);
struct tm *_gmtime32(const __time32_t *);
struct tm *_gmtime64(const __time64_t *);
struct tm *_localtime32(const __time32_t *);
struct tm *_localtime64(const __time64_t *);
__time32_t _mkgmtime32(struct tm *);
__time64_t _mkgmtime64(struct tm *);
__time32_t _mktime32(struct tm *);
__time64_t _mktime64(struct tm *);
size_t     strftime(char *, size_t, const char *, const struct tm *);
size_t     _strftime_l(char *, size_t, const char *, const struct tm *,
                       _locale_t);
char      *_strdate(char *);
char      *_strtime(char *);
__time32_t _time32(__time32_t *);
__time64_t _time64(__time64_t *);
int        _timespec32_get(struct _timespec32 *, int);
int        _timespec64_get(struct _timespec64 *, int);

void         _tzset(void);
unsigned int _getsystime(struct tm *);
unsigned int _setsystime(struct tm *, unsigned int);

char     *ctime(const time_t *);
double    difftime(time_t, time_t);
struct tm *gmtime(const time_t *);
struct tm *localtime(const time_t *);
time_t     mktime(struct tm *);
time_t     time(time_t *);
int        timespec_get(struct timespec *, int);

#include <bits/msvc/secure/time.h>

#endif
