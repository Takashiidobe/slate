#include <stdlib.h>

extern int slate_oracle_atoi(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atoi), __typeof__(atoi)),
    "stdlib.h:atoi declaration differs from oracle");

static __typeof__(atoi) *const slate_reference_atoi = &atoi;

extern char * slate_oracle_mktemp(char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mktemp), __typeof__(mktemp)),
    "stdlib.h:mktemp declaration differs from oracle");

static __typeof__(mktemp) *const slate_reference_mktemp = &mktemp;

#ifndef EXIT_FAILURE
#error "stdlib.h:EXIT_FAILURE macro is missing from libc-shim"
#endif

#ifndef EXIT_SUCCESS
#error "stdlib.h:EXIT_SUCCESS macro is missing from libc-shim"
#endif

#ifndef MB_CUR_MAX
#error "stdlib.h:MB_CUR_MAX macro is missing from libc-shim"
#endif

#ifndef NULL
#error "stdlib.h:NULL macro is missing from libc-shim"
#endif

#ifndef RAND_MAX
#error "stdlib.h:RAND_MAX macro is missing from libc-shim"
#endif

#ifndef WCOREDUMP
#error "stdlib.h:WCOREDUMP macro is missing from libc-shim"
#endif

#ifndef WEXITSTATUS
#error "stdlib.h:WEXITSTATUS macro is missing from libc-shim"
#endif

#ifndef WIFCONTINUED
#error "stdlib.h:WIFCONTINUED macro is missing from libc-shim"
#endif

#ifndef WIFEXITED
#error "stdlib.h:WIFEXITED macro is missing from libc-shim"
#endif

#ifndef WIFSIGNALED
#error "stdlib.h:WIFSIGNALED macro is missing from libc-shim"
#endif

#ifndef WIFSTOPPED
#error "stdlib.h:WIFSTOPPED macro is missing from libc-shim"
#endif

#ifndef WNOHANG
#error "stdlib.h:WNOHANG macro is missing from libc-shim"
#endif

#ifndef WSTOPSIG
#error "stdlib.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "stdlib.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef WUNTRACED
#error "stdlib.h:WUNTRACED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
