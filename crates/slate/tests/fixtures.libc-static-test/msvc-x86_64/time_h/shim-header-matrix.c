#include <time.h>

extern int * slate_oracle___daylight(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___daylight), __typeof__(__daylight)),
    "time.h:__daylight declaration differs from oracle");

static __typeof__(__daylight) *const slate_reference___daylight = &__daylight;

extern long * slate_oracle___dstbias(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___dstbias), __typeof__(__dstbias)),
    "time.h:__dstbias declaration differs from oracle");

static __typeof__(__dstbias) *const slate_reference___dstbias = &__dstbias;

extern long * slate_oracle___timezone(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___timezone), __typeof__(__timezone)),
    "time.h:__timezone declaration differs from oracle");

static __typeof__(__timezone) *const slate_reference___timezone = &__timezone;

extern char ** slate_oracle___tzname(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___tzname), __typeof__(__tzname)),
    "time.h:__tzname declaration differs from oracle");

static __typeof__(__tzname) *const slate_reference___tzname = &__tzname;

extern char * slate_oracle__ctime32(const long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ctime32), __typeof__(_ctime32)),
    "time.h:_ctime32 declaration differs from oracle");

static __typeof__(_ctime32) *const slate_reference__ctime32 = &_ctime32;

extern int slate_oracle__ctime32_s(char *, unsigned long long, const long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ctime32_s), __typeof__(_ctime32_s)),
    "time.h:_ctime32_s declaration differs from oracle");

static __typeof__(_ctime32_s) *const slate_reference__ctime32_s = &_ctime32_s;

extern char * slate_oracle__ctime64(const long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ctime64), __typeof__(_ctime64)),
    "time.h:_ctime64 declaration differs from oracle");

static __typeof__(_ctime64) *const slate_reference__ctime64 = &_ctime64;

extern int slate_oracle__ctime64_s(char *, unsigned long long, const long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ctime64_s), __typeof__(_ctime64_s)),
    "time.h:_ctime64_s declaration differs from oracle");

static __typeof__(_ctime64_s) *const slate_reference__ctime64_s = &_ctime64_s;

extern double slate_oracle__difftime32(long, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__difftime32), __typeof__(_difftime32)),
    "time.h:_difftime32 declaration differs from oracle");

static __typeof__(_difftime32) *const slate_reference__difftime32 = &_difftime32;

extern double slate_oracle__difftime64(long long, long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__difftime64), __typeof__(_difftime64)),
    "time.h:_difftime64 declaration differs from oracle");

static __typeof__(_difftime64) *const slate_reference__difftime64 = &_difftime64;

extern int slate_oracle__get_daylight(int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_daylight), __typeof__(_get_daylight)),
    "time.h:_get_daylight declaration differs from oracle");

static __typeof__(_get_daylight) *const slate_reference__get_daylight = &_get_daylight;

extern int slate_oracle__get_dstbias(long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_dstbias), __typeof__(_get_dstbias)),
    "time.h:_get_dstbias declaration differs from oracle");

static __typeof__(_get_dstbias) *const slate_reference__get_dstbias = &_get_dstbias;

extern int slate_oracle__get_timezone(long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_timezone), __typeof__(_get_timezone)),
    "time.h:_get_timezone declaration differs from oracle");

static __typeof__(_get_timezone) *const slate_reference__get_timezone = &_get_timezone;

extern int slate_oracle__get_tzname(unsigned long long *, char *, unsigned long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_tzname), __typeof__(_get_tzname)),
    "time.h:_get_tzname declaration differs from oracle");

static __typeof__(_get_tzname) *const slate_reference__get_tzname = &_get_tzname;

extern unsigned int slate_oracle__getsystime(struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__getsystime), __typeof__(_getsystime)),
    "time.h:_getsystime declaration differs from oracle");

static __typeof__(_getsystime) *const slate_reference__getsystime = &_getsystime;

extern struct tm * slate_oracle__gmtime32(const long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__gmtime32), __typeof__(_gmtime32)),
    "time.h:_gmtime32 declaration differs from oracle");

static __typeof__(_gmtime32) *const slate_reference__gmtime32 = &_gmtime32;

extern int slate_oracle__gmtime32_s(struct tm *, const long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__gmtime32_s), __typeof__(_gmtime32_s)),
    "time.h:_gmtime32_s declaration differs from oracle");

static __typeof__(_gmtime32_s) *const slate_reference__gmtime32_s = &_gmtime32_s;

extern struct tm * slate_oracle__gmtime64(const long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__gmtime64), __typeof__(_gmtime64)),
    "time.h:_gmtime64 declaration differs from oracle");

static __typeof__(_gmtime64) *const slate_reference__gmtime64 = &_gmtime64;

extern int slate_oracle__gmtime64_s(struct tm *, const long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__gmtime64_s), __typeof__(_gmtime64_s)),
    "time.h:_gmtime64_s declaration differs from oracle");

static __typeof__(_gmtime64_s) *const slate_reference__gmtime64_s = &_gmtime64_s;

extern struct tm * slate_oracle__localtime32(const long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__localtime32), __typeof__(_localtime32)),
    "time.h:_localtime32 declaration differs from oracle");

static __typeof__(_localtime32) *const slate_reference__localtime32 = &_localtime32;

extern int slate_oracle__localtime32_s(struct tm *, const long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__localtime32_s), __typeof__(_localtime32_s)),
    "time.h:_localtime32_s declaration differs from oracle");

static __typeof__(_localtime32_s) *const slate_reference__localtime32_s = &_localtime32_s;

extern struct tm * slate_oracle__localtime64(const long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__localtime64), __typeof__(_localtime64)),
    "time.h:_localtime64 declaration differs from oracle");

static __typeof__(_localtime64) *const slate_reference__localtime64 = &_localtime64;

extern int slate_oracle__localtime64_s(struct tm *, const long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__localtime64_s), __typeof__(_localtime64_s)),
    "time.h:_localtime64_s declaration differs from oracle");

static __typeof__(_localtime64_s) *const slate_reference__localtime64_s = &_localtime64_s;

extern long slate_oracle__mkgmtime32(struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mkgmtime32), __typeof__(_mkgmtime32)),
    "time.h:_mkgmtime32 declaration differs from oracle");

static __typeof__(_mkgmtime32) *const slate_reference__mkgmtime32 = &_mkgmtime32;

extern long long slate_oracle__mkgmtime64(struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mkgmtime64), __typeof__(_mkgmtime64)),
    "time.h:_mkgmtime64 declaration differs from oracle");

static __typeof__(_mkgmtime64) *const slate_reference__mkgmtime64 = &_mkgmtime64;

extern long slate_oracle__mktime32(struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mktime32), __typeof__(_mktime32)),
    "time.h:_mktime32 declaration differs from oracle");

static __typeof__(_mktime32) *const slate_reference__mktime32 = &_mktime32;

extern long long slate_oracle__mktime64(struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mktime64), __typeof__(_mktime64)),
    "time.h:_mktime64 declaration differs from oracle");

static __typeof__(_mktime64) *const slate_reference__mktime64 = &_mktime64;

extern unsigned int slate_oracle__setsystime(struct tm *, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__setsystime), __typeof__(_setsystime)),
    "time.h:_setsystime declaration differs from oracle");

static __typeof__(_setsystime) *const slate_reference__setsystime = &_setsystime;

extern char * slate_oracle__strdate(char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strdate), __typeof__(_strdate)),
    "time.h:_strdate declaration differs from oracle");

static __typeof__(_strdate) *const slate_reference__strdate = &_strdate;

extern int slate_oracle__strdate_s(char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strdate_s), __typeof__(_strdate_s)),
    "time.h:_strdate_s declaration differs from oracle");

static __typeof__(_strdate_s) *const slate_reference__strdate_s = &_strdate_s;

extern unsigned long long slate_oracle__strftime_l(char *, unsigned long long, const char *, const struct tm *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strftime_l), __typeof__(_strftime_l)),
    "time.h:_strftime_l declaration differs from oracle");

static __typeof__(_strftime_l) *const slate_reference__strftime_l = &_strftime_l;

extern char * slate_oracle__strtime(char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtime), __typeof__(_strtime)),
    "time.h:_strtime declaration differs from oracle");

static __typeof__(_strtime) *const slate_reference__strtime = &_strtime;

extern int slate_oracle__strtime_s(char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtime_s), __typeof__(_strtime_s)),
    "time.h:_strtime_s declaration differs from oracle");

static __typeof__(_strtime_s) *const slate_reference__strtime_s = &_strtime_s;

extern long slate_oracle__time32(long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__time32), __typeof__(_time32)),
    "time.h:_time32 declaration differs from oracle");

static __typeof__(_time32) *const slate_reference__time32 = &_time32;

extern long long slate_oracle__time64(long long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__time64), __typeof__(_time64)),
    "time.h:_time64 declaration differs from oracle");

static __typeof__(_time64) *const slate_reference__time64 = &_time64;

extern int slate_oracle__timespec32_get(struct _timespec32 *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__timespec32_get), __typeof__(_timespec32_get)),
    "time.h:_timespec32_get declaration differs from oracle");

static __typeof__(_timespec32_get) *const slate_reference__timespec32_get = &_timespec32_get;

extern int slate_oracle__timespec64_get(struct _timespec64 *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__timespec64_get), __typeof__(_timespec64_get)),
    "time.h:_timespec64_get declaration differs from oracle");

static __typeof__(_timespec64_get) *const slate_reference__timespec64_get = &_timespec64_get;

extern void slate_oracle__tzset(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__tzset), __typeof__(_tzset)),
    "time.h:_tzset declaration differs from oracle");

static __typeof__(_tzset) *const slate_reference__tzset = &_tzset;

extern char * slate_oracle_asctime(const struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asctime), __typeof__(asctime)),
    "time.h:asctime declaration differs from oracle");

static __typeof__(asctime) *const slate_reference_asctime = &asctime;

extern int slate_oracle_asctime_s(char *, unsigned long long, const struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asctime_s), __typeof__(asctime_s)),
    "time.h:asctime_s declaration differs from oracle");

static __typeof__(asctime_s) *const slate_reference_asctime_s = &asctime_s;

extern long slate_oracle_clock(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clock), __typeof__(clock)),
    "time.h:clock declaration differs from oracle");

static __typeof__(clock) *const slate_reference_clock = &clock;

extern unsigned long long slate_oracle_strftime(char *, unsigned long long, const char *, const struct tm *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strftime), __typeof__(strftime)),
    "time.h:strftime declaration differs from oracle");

static __typeof__(strftime) *const slate_reference_strftime = &strftime;

extern void slate_oracle_tzset(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tzset), __typeof__(tzset)),
    "time.h:tzset declaration differs from oracle");

static __typeof__(tzset) *const slate_reference_tzset = &tzset;

typedef long slate_oracle_typedef_clock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clock_t, clock_t), "typedef clock_t differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

#ifndef CLK_TCK
#error "time.h:CLK_TCK macro is missing from libc-shim"
#endif

#ifndef CLOCKS_PER_SEC
#error "time.h:CLOCKS_PER_SEC macro is missing from libc-shim"
#endif

#ifndef TIME_UTC
#error "time.h:TIME_UTC macro is missing from libc-shim"
#endif

#ifndef _INC_TIME
#error "time.h:_INC_TIME macro is missing from libc-shim"
#endif

#ifndef _daylight
#error "time.h:_daylight macro is missing from libc-shim"
#endif

#ifndef _dstbias
#error "time.h:_dstbias macro is missing from libc-shim"
#endif

#ifndef _timezone
#error "time.h:_timezone macro is missing from libc-shim"
#endif

#ifndef _tzname
#error "time.h:_tzname macro is missing from libc-shim"
#endif

int main(void) { return 0; }
