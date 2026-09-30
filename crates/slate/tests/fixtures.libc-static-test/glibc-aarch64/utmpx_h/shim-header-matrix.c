#include <utmpx.h>

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

#ifndef ACCOUNTING
#error "utmpx.h:ACCOUNTING macro is missing from libc-shim"
#endif

#ifndef BOOT_TIME
#error "utmpx.h:BOOT_TIME macro is missing from libc-shim"
#endif

#ifndef DEAD_PROCESS
#error "utmpx.h:DEAD_PROCESS macro is missing from libc-shim"
#endif

#ifndef EMPTY
#error "utmpx.h:EMPTY macro is missing from libc-shim"
#endif

#ifndef INIT_PROCESS
#error "utmpx.h:INIT_PROCESS macro is missing from libc-shim"
#endif

#ifndef LOGIN_PROCESS
#error "utmpx.h:LOGIN_PROCESS macro is missing from libc-shim"
#endif

#ifndef NEW_TIME
#error "utmpx.h:NEW_TIME macro is missing from libc-shim"
#endif

#ifndef OLD_TIME
#error "utmpx.h:OLD_TIME macro is missing from libc-shim"
#endif

#ifndef RUN_LVL
#error "utmpx.h:RUN_LVL macro is missing from libc-shim"
#endif

#ifndef USER_PROCESS
#error "utmpx.h:USER_PROCESS macro is missing from libc-shim"
#endif

#ifndef UTMPX_FILE
#error "utmpx.h:UTMPX_FILE macro is missing from libc-shim"
#endif

#ifndef UTMPX_FILENAME
#error "utmpx.h:UTMPX_FILENAME macro is missing from libc-shim"
#endif

#ifndef WTMPX_FILE
#error "utmpx.h:WTMPX_FILE macro is missing from libc-shim"
#endif

#ifndef WTMPX_FILENAME
#error "utmpx.h:WTMPX_FILENAME macro is missing from libc-shim"
#endif

int main(void) { return 0; }
