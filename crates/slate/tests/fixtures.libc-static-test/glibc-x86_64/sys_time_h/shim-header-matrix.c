#include <sys/time.h>

typedef long slate_oracle_typedef_suseconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_suseconds_t, suseconds_t), "typedef suseconds_t differs from oracle");

_Static_assert(sizeof(struct timezone) == 8, "struct timezone size differs from oracle");

_Static_assert(_Alignof(struct timezone) == 4, "struct timezone alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timezone, tz_minuteswest) == 0, "struct timezone.tz_minuteswest offset differs from oracle");

typedef int slate_oracle_struct_timezone_tz_minuteswest;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timezone *)0)->tz_minuteswest), slate_oracle_struct_timezone_tz_minuteswest), "struct timezone.tz_minuteswest field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timezone, tz_dsttime) == 4, "struct timezone.tz_dsttime offset differs from oracle");

typedef int slate_oracle_struct_timezone_tz_dsttime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timezone *)0)->tz_dsttime), slate_oracle_struct_timezone_tz_dsttime), "struct timezone.tz_dsttime field type differs from oracle");

#ifndef ITIMER_PROF
#error "sys/time.h:ITIMER_PROF macro is missing from libc-shim"
#endif

#ifndef ITIMER_REAL
#error "sys/time.h:ITIMER_REAL macro is missing from libc-shim"
#endif

#ifndef ITIMER_VIRTUAL
#error "sys/time.h:ITIMER_VIRTUAL macro is missing from libc-shim"
#endif

#ifndef TIMESPEC_TO_TIMEVAL
#error "sys/time.h:TIMESPEC_TO_TIMEVAL macro is missing from libc-shim"
#endif

#ifndef TIMEVAL_TO_TIMESPEC
#error "sys/time.h:TIMEVAL_TO_TIMESPEC macro is missing from libc-shim"
#endif

#ifndef timeradd
#error "sys/time.h:timeradd macro is missing from libc-shim"
#endif

#ifndef timerclear
#error "sys/time.h:timerclear macro is missing from libc-shim"
#endif

#ifndef timercmp
#error "sys/time.h:timercmp macro is missing from libc-shim"
#endif

#ifndef timerisset
#error "sys/time.h:timerisset macro is missing from libc-shim"
#endif

#ifndef timersub
#error "sys/time.h:timersub macro is missing from libc-shim"
#endif

int main(void) { return 0; }
