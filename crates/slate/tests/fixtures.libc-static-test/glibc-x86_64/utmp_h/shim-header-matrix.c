#include <utmp.h>

extern int slate_oracle_login_tty(int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_login_tty), __typeof__(login_tty)),
    "utmp.h:login_tty declaration differs from oracle");

static __typeof__(login_tty) *const slate_reference_login_tty = &login_tty;

_Static_assert(sizeof(struct lastlog) == 292, "struct lastlog size differs from oracle");

_Static_assert(_Alignof(struct lastlog) == 4, "struct lastlog alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct lastlog, ll_time) == 0, "struct lastlog.ll_time offset differs from oracle");

typedef unsigned int slate_oracle_struct_lastlog_ll_time;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct lastlog *)0)->ll_time), slate_oracle_struct_lastlog_ll_time), "struct lastlog.ll_time field type differs from oracle");

_Static_assert(__builtin_offsetof(struct lastlog, ll_line) == 4, "struct lastlog.ll_line offset differs from oracle");

_Static_assert(__builtin_offsetof(struct lastlog, ll_host) == 36, "struct lastlog.ll_host offset differs from oracle");

#ifndef ACCOUNTING
#error "utmp.h:ACCOUNTING macro is missing from libc-shim"
#endif

#ifndef BOOT_TIME
#error "utmp.h:BOOT_TIME macro is missing from libc-shim"
#endif

#ifndef DEAD_PROCESS
#error "utmp.h:DEAD_PROCESS macro is missing from libc-shim"
#endif

#ifndef EMPTY
#error "utmp.h:EMPTY macro is missing from libc-shim"
#endif

#ifndef INIT_PROCESS
#error "utmp.h:INIT_PROCESS macro is missing from libc-shim"
#endif

#ifndef LOGIN_PROCESS
#error "utmp.h:LOGIN_PROCESS macro is missing from libc-shim"
#endif

#ifndef NEW_TIME
#error "utmp.h:NEW_TIME macro is missing from libc-shim"
#endif

#ifndef OLD_TIME
#error "utmp.h:OLD_TIME macro is missing from libc-shim"
#endif

#ifndef RUN_LVL
#error "utmp.h:RUN_LVL macro is missing from libc-shim"
#endif

#ifndef USER_PROCESS
#error "utmp.h:USER_PROCESS macro is missing from libc-shim"
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

#ifndef UT_UNKNOWN
#error "utmp.h:UT_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef WTMP_FILE
#error "utmp.h:WTMP_FILE macro is missing from libc-shim"
#endif

#ifndef WTMP_FILENAME
#error "utmp.h:WTMP_FILENAME macro is missing from libc-shim"
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

#ifndef ut_xtime
#error "utmp.h:ut_xtime macro is missing from libc-shim"
#endif

int main(void) { return 0; }
