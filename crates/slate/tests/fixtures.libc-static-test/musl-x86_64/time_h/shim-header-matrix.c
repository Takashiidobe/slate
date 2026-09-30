#include <time.h>

_Static_assert(sizeof(struct tm) == 56, "struct tm size differs from oracle");

_Static_assert(_Alignof(struct tm) == 8, "struct tm alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_sec) == 0, "struct tm.tm_sec offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_sec), slate_oracle_struct_tm_tm_sec), "struct tm.tm_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_min) == 4, "struct tm.tm_min offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_min;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_min), slate_oracle_struct_tm_tm_min), "struct tm.tm_min field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_hour) == 8, "struct tm.tm_hour offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_hour;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_hour), slate_oracle_struct_tm_tm_hour), "struct tm.tm_hour field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_mday) == 12, "struct tm.tm_mday offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_mday;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_mday), slate_oracle_struct_tm_tm_mday), "struct tm.tm_mday field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_mon) == 16, "struct tm.tm_mon offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_mon;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_mon), slate_oracle_struct_tm_tm_mon), "struct tm.tm_mon field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_year) == 20, "struct tm.tm_year offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_year;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_year), slate_oracle_struct_tm_tm_year), "struct tm.tm_year field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_wday) == 24, "struct tm.tm_wday offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_wday;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_wday), slate_oracle_struct_tm_tm_wday), "struct tm.tm_wday field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_yday) == 28, "struct tm.tm_yday offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_yday;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_yday), slate_oracle_struct_tm_tm_yday), "struct tm.tm_yday field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_isdst) == 32, "struct tm.tm_isdst offset differs from oracle");

typedef int slate_oracle_struct_tm_tm_isdst;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_isdst), slate_oracle_struct_tm_tm_isdst), "struct tm.tm_isdst field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_gmtoff) == 40, "struct tm.tm_gmtoff offset differs from oracle");

typedef long slate_oracle_struct_tm_tm_gmtoff;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_gmtoff), slate_oracle_struct_tm_tm_gmtoff), "struct tm.tm_gmtoff field type differs from oracle");

_Static_assert(__builtin_offsetof(struct tm, tm_zone) == 48, "struct tm.tm_zone offset differs from oracle");

typedef const char * slate_oracle_struct_tm_tm_zone;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tm *)0)->tm_zone), slate_oracle_struct_tm_tm_zone), "struct tm.tm_zone field type differs from oracle");

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

#ifndef CLOCK_SGI_CYCLE
#error "time.h:CLOCK_SGI_CYCLE macro is missing from libc-shim"
#endif

#ifndef CLOCK_TAI
#error "time.h:CLOCK_TAI macro is missing from libc-shim"
#endif

#ifndef CLOCK_THREAD_CPUTIME_ID
#error "time.h:CLOCK_THREAD_CPUTIME_ID macro is missing from libc-shim"
#endif

#ifndef NULL
#error "time.h:NULL macro is missing from libc-shim"
#endif

#ifndef TIMER_ABSTIME
#error "time.h:TIMER_ABSTIME macro is missing from libc-shim"
#endif

#ifndef TIME_UTC
#error "time.h:TIME_UTC macro is missing from libc-shim"
#endif

int main(void) { return 0; }
