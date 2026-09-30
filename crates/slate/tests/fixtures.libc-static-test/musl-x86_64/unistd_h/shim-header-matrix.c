#include <unistd.h>

extern int slate_oracle_pipe(int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pipe), __typeof__(pipe)),
    "unistd.h:pipe declaration differs from oracle");

static __typeof__(pipe) *const slate_reference_pipe = &pipe;

extern char ** slate_oracle_environ;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_environ), __typeof__(environ)), "environ object type differs from oracle");

static __typeof__(environ) *const slate_reference_environ = &environ;

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

#ifndef NULL
#error "unistd.h:NULL macro is missing from libc-shim"
#endif

#ifndef POSIX_CLOSE_RESTART
#error "unistd.h:POSIX_CLOSE_RESTART macro is missing from libc-shim"
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

#ifndef W_OK
#error "unistd.h:W_OK macro is missing from libc-shim"
#endif

#ifndef X_OK
#error "unistd.h:X_OK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
