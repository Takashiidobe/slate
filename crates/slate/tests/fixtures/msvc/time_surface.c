#include <stddef.h>
#include <time.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(sizeof(__time32_t) == 4, "__time32_t size");
_Static_assert(_Alignof(__time32_t) == 4, "__time32_t alignment");
_Static_assert(sizeof(__time64_t) == 8, "__time64_t size");
_Static_assert(_Alignof(__time64_t) == 8, "__time64_t alignment");
_Static_assert(sizeof(time_t) == 8, "time_t size");
_Static_assert(sizeof(clock_t) == 4, "clock_t size");

_Static_assert(sizeof(struct tm) == 36, "tm size");
_Static_assert(_Alignof(struct tm) == 4, "tm alignment");
_Static_assert(offsetof(struct tm, tm_sec) == 0, "tm sec");
_Static_assert(offsetof(struct tm, tm_min) == 4, "tm min");
_Static_assert(offsetof(struct tm, tm_hour) == 8, "tm hour");
_Static_assert(offsetof(struct tm, tm_mday) == 12, "tm mday");
_Static_assert(offsetof(struct tm, tm_mon) == 16, "tm mon");
_Static_assert(offsetof(struct tm, tm_year) == 20, "tm year");
_Static_assert(offsetof(struct tm, tm_wday) == 24, "tm wday");
_Static_assert(offsetof(struct tm, tm_yday) == 28, "tm yday");
_Static_assert(offsetof(struct tm, tm_isdst) == 32, "tm isdst");
TYPE_IS(((struct tm *)0)->tm_sec, int);
TYPE_IS(((struct tm *)0)->tm_min, int);
TYPE_IS(((struct tm *)0)->tm_hour, int);
TYPE_IS(((struct tm *)0)->tm_mday, int);
TYPE_IS(((struct tm *)0)->tm_mon, int);
TYPE_IS(((struct tm *)0)->tm_year, int);
TYPE_IS(((struct tm *)0)->tm_wday, int);
TYPE_IS(((struct tm *)0)->tm_yday, int);
TYPE_IS(((struct tm *)0)->tm_isdst, int);

_Static_assert(sizeof(struct _timespec32) == 8, "_timespec32 size");
_Static_assert(_Alignof(struct _timespec32) == 4, "_timespec32 alignment");
_Static_assert(offsetof(struct _timespec32, tv_sec) == 0, "_timespec32 sec");
_Static_assert(offsetof(struct _timespec32, tv_nsec) == 4, "_timespec32 nsec");
_Static_assert(sizeof(struct _timespec64) == 16, "_timespec64 size");
_Static_assert(_Alignof(struct _timespec64) == 8, "_timespec64 alignment");
_Static_assert(offsetof(struct _timespec64, tv_sec) == 0, "_timespec64 sec");
_Static_assert(offsetof(struct _timespec64, tv_nsec) == 8, "_timespec64 nsec");
_Static_assert(sizeof(struct timespec) == 16, "timespec size");
_Static_assert(_Alignof(struct timespec) == 8, "timespec alignment");
_Static_assert(offsetof(struct timespec, tv_sec) == 0, "timespec sec");
_Static_assert(offsetof(struct timespec, tv_nsec) == 8, "timespec nsec");

TYPE_IS(((struct _timespec32 *)0)->tv_sec, __time32_t);
TYPE_IS(((struct _timespec32 *)0)->tv_nsec, long);
TYPE_IS(((struct _timespec64 *)0)->tv_sec, __time64_t);
TYPE_IS(((struct _timespec64 *)0)->tv_nsec, long);
TYPE_IS(((struct timespec *)0)->tv_sec, time_t);
TYPE_IS(((struct timespec *)0)->tv_nsec, long);

_Static_assert(CLOCKS_PER_SEC == 1000, "CLOCKS_PER_SEC");
_Static_assert(TIME_UTC == 1, "TIME_UTC");

TYPE_IS(&__daylight, int *(*)(void));
TYPE_IS(&__dstbias, long *(*)(void));
TYPE_IS(&__timezone, long *(*)(void));
TYPE_IS(&__tzname, char **(*)(void));
TYPE_IS(&_get_daylight, errno_t (*)(int *));
TYPE_IS(&_get_dstbias, errno_t (*)(long *));
TYPE_IS(&_get_timezone, errno_t (*)(long *));
TYPE_IS(&_get_tzname, errno_t (*)(size_t *, char *, size_t, int));

TYPE_IS(&_ctime32, char *(*)(const __time32_t *));
TYPE_IS(&_ctime64, char *(*)(const __time64_t *));
TYPE_IS(&_difftime32, double (*)(__time32_t, __time32_t));
TYPE_IS(&_difftime64, double (*)(__time64_t, __time64_t));
TYPE_IS(&_gmtime32, struct tm *(*)(const __time32_t *));
TYPE_IS(&_gmtime64, struct tm *(*)(const __time64_t *));
TYPE_IS(&_localtime32, struct tm *(*)(const __time32_t *));
TYPE_IS(&_localtime64, struct tm *(*)(const __time64_t *));
TYPE_IS(&_mkgmtime32, __time32_t (*)(struct tm *));
TYPE_IS(&_mkgmtime64, __time64_t (*)(struct tm *));
TYPE_IS(&_mktime32, __time32_t (*)(struct tm *));
TYPE_IS(&_mktime64, __time64_t (*)(struct tm *));
TYPE_IS(&_time32, __time32_t (*)(__time32_t *));
TYPE_IS(&_time64, __time64_t (*)(__time64_t *));
TYPE_IS(&_timespec32_get, int (*)(struct _timespec32 *, int));
TYPE_IS(&_timespec64_get, int (*)(struct _timespec64 *, int));
TYPE_IS(&_strftime_l,
        size_t (*)(char *, size_t, const char *, const struct tm *, _locale_t));
TYPE_IS(&_strdate, char *(*)(char *));
TYPE_IS(&_strtime, char *(*)(char *));
TYPE_IS(&_tzset, void (*)(void));
TYPE_IS(&_getsystime, unsigned int (*)(struct tm *));
TYPE_IS(&_setsystime, unsigned int (*)(struct tm *, unsigned int));

TYPE_IS(&asctime, char *(*)(const struct tm *));
TYPE_IS(&clock, clock_t (*)(void));
TYPE_IS(&ctime, char *(*)(const time_t *));
TYPE_IS(&difftime, double (*)(time_t, time_t));
TYPE_IS(&gmtime, struct tm *(*)(const time_t *));
TYPE_IS(&localtime, struct tm *(*)(const time_t *));
TYPE_IS(&mktime, time_t (*)(struct tm *));
TYPE_IS(&strftime,
        size_t (*)(char *, size_t, const char *, const struct tm *));
TYPE_IS(&time, time_t (*)(time_t *));
TYPE_IS(&timespec_get, int (*)(struct timespec *, int));

extern int timespec_getres;
extern int gmtime_r;
extern int localtime_r;

int main(void) { return 0; }
