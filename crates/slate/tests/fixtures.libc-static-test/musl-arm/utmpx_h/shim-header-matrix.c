#include <utmpx.h>

_Static_assert(__builtin_offsetof(struct utmpx, ut_type) == 0, "struct utmpx.ut_type offset differs from oracle");

typedef short slate_oracle_struct_utmpx_ut_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utmpx *)0)->ut_type), slate_oracle_struct_utmpx_ut_type), "struct utmpx.ut_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct utmpx, __ut_pad1) == 2, "struct utmpx.__ut_pad1 offset differs from oracle");

typedef short slate_oracle_struct_utmpx___ut_pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utmpx *)0)->__ut_pad1), slate_oracle_struct_utmpx___ut_pad1), "struct utmpx.__ut_pad1 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct utmpx, ut_pid) == 4, "struct utmpx.ut_pid offset differs from oracle");

typedef int slate_oracle_struct_utmpx_ut_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utmpx *)0)->ut_pid), slate_oracle_struct_utmpx_ut_pid), "struct utmpx.ut_pid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct utmpx, ut_line) == 8, "struct utmpx.ut_line offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utmpx, ut_id) == 40, "struct utmpx.ut_id offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utmpx, ut_user) == 44, "struct utmpx.ut_user offset differs from oracle");

_Static_assert(__builtin_offsetof(struct utmpx, ut_host) == 76, "struct utmpx.ut_host offset differs from oracle");

typedef int slate_oracle_struct_utmpx_ut_session;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utmpx *)0)->ut_session), slate_oracle_struct_utmpx_ut_session), "struct utmpx.ut_session field type differs from oracle");

typedef int slate_oracle_struct_utmpx___ut_pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utmpx *)0)->__ut_pad2), slate_oracle_struct_utmpx___ut_pad2), "struct utmpx.__ut_pad2 field type differs from oracle");

typedef struct timeval slate_oracle_struct_utmpx_ut_tv;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct utmpx *)0)->ut_tv), slate_oracle_struct_utmpx_ut_tv), "struct utmpx.ut_tv field type differs from oracle");

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

#ifndef e_exit
#error "utmpx.h:e_exit macro is missing from libc-shim"
#endif

#ifndef e_termination
#error "utmpx.h:e_termination macro is missing from libc-shim"
#endif

int main(void) { return 0; }
