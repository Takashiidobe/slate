#include <sys/time.h>

extern int slate_oracle_gettimeofday(struct timeval *restrict, void *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gettimeofday), __typeof__(gettimeofday)),
    "sys/time.h:gettimeofday declaration differs from oracle");

static __typeof__(gettimeofday) *const slate_reference_gettimeofday = &gettimeofday;

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
