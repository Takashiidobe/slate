#include <spawn.h>

#ifndef POSIX_SPAWN_RESETIDS
#error "spawn.h:POSIX_SPAWN_RESETIDS macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETPGROUP
#error "spawn.h:POSIX_SPAWN_SETPGROUP macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETSCHEDPARAM
#error "spawn.h:POSIX_SPAWN_SETSCHEDPARAM macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETSCHEDULER
#error "spawn.h:POSIX_SPAWN_SETSCHEDULER macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETSID
#error "spawn.h:POSIX_SPAWN_SETSID macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETSIGDEF
#error "spawn.h:POSIX_SPAWN_SETSIGDEF macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETSIGMASK
#error "spawn.h:POSIX_SPAWN_SETSIGMASK macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_USEVFORK
#error "spawn.h:POSIX_SPAWN_USEVFORK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
