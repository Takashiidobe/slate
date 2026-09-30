#include <time.h>

extern long slate_oracle_clock(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clock), __typeof__(clock)),
    "time.h:clock declaration differs from oracle");

static __typeof__(clock) *const slate_reference_clock = &clock;

extern int slate_oracle_clock_adjtime(int, struct timex *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clock_adjtime), __typeof__(clock_adjtime)),
    "time.h:clock_adjtime declaration differs from oracle");

static __typeof__(clock_adjtime) *const slate_reference_clock_adjtime = &clock_adjtime;

#ifndef CLOCKS_PER_SEC
#error "time.h:CLOCKS_PER_SEC macro is missing from libc-shim"
#endif

#ifndef CLOCK_BOOTTIME
#error "time.h:CLOCK_BOOTTIME macro is missing from libc-shim"
#endif

#ifndef CLOCK_BOOTTIME_ALARM
#error "time.h:CLOCK_BOOTTIME_ALARM macro is missing from libc-shim"
#endif

#ifndef CLOCK_MONOTONIC
#error "time.h:CLOCK_MONOTONIC macro is missing from libc-shim"
#endif

#ifndef CLOCK_MONOTONIC_COARSE
#error "time.h:CLOCK_MONOTONIC_COARSE macro is missing from libc-shim"
#endif

#ifndef CLOCK_MONOTONIC_RAW
#error "time.h:CLOCK_MONOTONIC_RAW macro is missing from libc-shim"
#endif

#ifndef CLOCK_PROCESS_CPUTIME_ID
#error "time.h:CLOCK_PROCESS_CPUTIME_ID macro is missing from libc-shim"
#endif

#ifndef CLOCK_REALTIME
#error "time.h:CLOCK_REALTIME macro is missing from libc-shim"
#endif

#ifndef CLOCK_REALTIME_ALARM
#error "time.h:CLOCK_REALTIME_ALARM macro is missing from libc-shim"
#endif

#ifndef CLOCK_REALTIME_COARSE
#error "time.h:CLOCK_REALTIME_COARSE macro is missing from libc-shim"
#endif

#ifndef CLOCK_TAI
#error "time.h:CLOCK_TAI macro is missing from libc-shim"
#endif

#ifndef CLOCK_THREAD_CPUTIME_ID
#error "time.h:CLOCK_THREAD_CPUTIME_ID macro is missing from libc-shim"
#endif

#ifndef TIMER_ABSTIME
#error "time.h:TIMER_ABSTIME macro is missing from libc-shim"
#endif

#ifndef TIME_ACTIVE
#error "time.h:TIME_ACTIVE macro is missing from libc-shim"
#endif

#ifndef TIME_MONOTONIC
#error "time.h:TIME_MONOTONIC macro is missing from libc-shim"
#endif

#ifndef TIME_THREAD_ACTIVE
#error "time.h:TIME_THREAD_ACTIVE macro is missing from libc-shim"
#endif

#ifndef TIME_UTC
#error "time.h:TIME_UTC macro is missing from libc-shim"
#endif

int main(void) { return 0; }
