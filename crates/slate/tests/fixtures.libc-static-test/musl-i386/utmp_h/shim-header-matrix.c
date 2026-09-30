#include <utmp.h>

_Static_assert(sizeof(struct lastlog) == 296, "struct lastlog size differs from oracle");

_Static_assert(_Alignof(struct lastlog) == 4, "struct lastlog alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct lastlog, ll_time) == 0, "struct lastlog.ll_time offset differs from oracle");

typedef long long slate_oracle_struct_lastlog_ll_time;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lastlog *)0)->ll_time), slate_oracle_struct_lastlog_ll_time), "struct lastlog.ll_time field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lastlog, ll_line) == 8, "struct lastlog.ll_line offset differs from oracle");

_Static_assert(__builtin_offsetof(struct lastlog, ll_host) == 40, "struct lastlog.ll_host offset differs from oracle");

#ifndef ACCOUNTING
#error "utmp.h:ACCOUNTING macro is missing from libc-shim"
#endif

#ifndef UTMP_FILE
#error "utmp.h:UTMP_FILE macro is missing from libc-shim"
#endif

#ifndef UTMP_FILENAME
#error "utmp.h:UTMP_FILENAME macro is missing from libc-shim"
#endif

#ifndef UT_HOSTSIZE
#error "utmp.h:UT_HOSTSIZE macro is missing from libc-shim"
#endif

#ifndef UT_LINESIZE
#error "utmp.h:UT_LINESIZE macro is missing from libc-shim"
#endif

#ifndef UT_NAMESIZE
#error "utmp.h:UT_NAMESIZE macro is missing from libc-shim"
#endif

#ifndef WTMP_FILE
#error "utmp.h:WTMP_FILE macro is missing from libc-shim"
#endif

#ifndef WTMP_FILENAME
#error "utmp.h:WTMP_FILENAME macro is missing from libc-shim"
#endif

#ifndef e_exit
#error "utmp.h:e_exit macro is missing from libc-shim"
#endif

#ifndef e_termination
#error "utmp.h:e_termination macro is missing from libc-shim"
#endif

#ifndef ut_addr
#error "utmp.h:ut_addr macro is missing from libc-shim"
#endif

#ifndef ut_name
#error "utmp.h:ut_name macro is missing from libc-shim"
#endif

#ifndef ut_time
#error "utmp.h:ut_time macro is missing from libc-shim"
#endif

#ifndef utmp
#error "utmp.h:utmp macro is missing from libc-shim"
#endif

int main(void) { return 0; }
