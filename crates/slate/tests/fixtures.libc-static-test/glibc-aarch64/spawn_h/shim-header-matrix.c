#include <spawn.h>

extern int slate_oracle_posix_spawnattr_getcgroup_np(const posix_spawnattr_t *restrict, int *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_spawnattr_getcgroup_np), __typeof__(posix_spawnattr_getcgroup_np)),
    "spawn.h:posix_spawnattr_getcgroup_np declaration differs from oracle");

static __typeof__(posix_spawnattr_getcgroup_np) *const slate_reference_posix_spawnattr_getcgroup_np = &posix_spawnattr_getcgroup_np;

#ifndef POSIX_SPAWN_RESETIDS
#error "spawn.h:POSIX_SPAWN_RESETIDS macro is missing from libc-shim"
#endif

#ifndef POSIX_SPAWN_SETCGROUP
#error "spawn.h:POSIX_SPAWN_SETCGROUP macro is missing from libc-shim"
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
