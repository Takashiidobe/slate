#include <unistd.h>

extern int slate_oracle_gethostname(char *, unsigned long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gethostname), __typeof__(gethostname)),
    "unistd.h:gethostname declaration differs from oracle");

static __typeof__(gethostname) *const slate_reference_gethostname = &gethostname;

extern int slate_oracle_gettid(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gettid), __typeof__(gettid)),
    "unistd.h:gettid declaration differs from oracle");

static __typeof__(gettid) *const slate_reference_gettid = &gettid;

extern long slate_oracle_pathconf(const char *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pathconf), __typeof__(pathconf)),
    "unistd.h:pathconf declaration differs from oracle");

static __typeof__(pathconf) *const slate_reference_pathconf = &pathconf;

extern char ** slate_oracle_environ;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_environ), __typeof__(environ)), "environ object type differs from oracle");

static __typeof__(environ) *const slate_reference_environ = &environ;

typedef unsigned int slate_oracle_typedef_gid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_gid_t, gid_t), "typedef gid_t differs from oracle");

typedef long slate_oracle_typedef_ssize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ssize_t, ssize_t), "typedef ssize_t differs from oracle");

#ifndef F_LOCK
#error "unistd.h:F_LOCK macro is missing from libc-shim"
#endif

#ifndef F_OK
#error "unistd.h:F_OK macro is missing from libc-shim"
#endif

#ifndef F_TEST
#error "unistd.h:F_TEST macro is missing from libc-shim"
#endif

#ifndef F_TLOCK
#error "unistd.h:F_TLOCK macro is missing from libc-shim"
#endif

#ifndef F_ULOCK
#error "unistd.h:F_ULOCK macro is missing from libc-shim"
#endif

#ifndef L_INCR
#error "unistd.h:L_INCR macro is missing from libc-shim"
#endif

#ifndef L_SET
#error "unistd.h:L_SET macro is missing from libc-shim"
#endif

#ifndef L_XTND
#error "unistd.h:L_XTND macro is missing from libc-shim"
#endif

#ifndef R_OK
#error "unistd.h:R_OK macro is missing from libc-shim"
#endif

#ifndef SEEK_CUR
#error "unistd.h:SEEK_CUR macro is missing from libc-shim"
#endif

#ifndef SEEK_DATA
#error "unistd.h:SEEK_DATA macro is missing from libc-shim"
#endif

#ifndef SEEK_END
#error "unistd.h:SEEK_END macro is missing from libc-shim"
#endif

#ifndef SEEK_HOLE
#error "unistd.h:SEEK_HOLE macro is missing from libc-shim"
#endif

#ifndef SEEK_SET
#error "unistd.h:SEEK_SET macro is missing from libc-shim"
#endif

#ifndef STDERR_FILENO
#error "unistd.h:STDERR_FILENO macro is missing from libc-shim"
#endif

#ifndef STDIN_FILENO
#error "unistd.h:STDIN_FILENO macro is missing from libc-shim"
#endif

#ifndef STDOUT_FILENO
#error "unistd.h:STDOUT_FILENO macro is missing from libc-shim"
#endif

#ifndef TEMP_FAILURE_RETRY
#error "unistd.h:TEMP_FAILURE_RETRY macro is missing from libc-shim"
#endif

#ifndef W_OK
#error "unistd.h:W_OK macro is missing from libc-shim"
#endif

#ifndef X_OK
#error "unistd.h:X_OK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
