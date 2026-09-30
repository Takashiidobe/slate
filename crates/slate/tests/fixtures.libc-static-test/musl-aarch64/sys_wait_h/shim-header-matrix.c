#include <sys/wait.h>

extern int slate_oracle_wait3(int *, int, struct rusage *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wait3), __typeof__(wait3)),
    "sys/wait.h:wait3 declaration differs from oracle");

static __typeof__(wait3) *const slate_reference_wait3 = &wait3;

extern int slate_oracle_waitid(idtype_t, unsigned int, siginfo_t *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_waitid), __typeof__(waitid)),
    "sys/wait.h:waitid declaration differs from oracle");

static __typeof__(waitid) *const slate_reference_waitid = &waitid;

#ifndef WCONTINUED
#error "sys/wait.h:WCONTINUED macro is missing from libc-shim"
#endif

#ifndef WCOREDUMP
#error "sys/wait.h:WCOREDUMP macro is missing from libc-shim"
#endif

#ifndef WEXITED
#error "sys/wait.h:WEXITED macro is missing from libc-shim"
#endif

#ifndef WEXITSTATUS
#error "sys/wait.h:WEXITSTATUS macro is missing from libc-shim"
#endif

#ifndef WIFCONTINUED
#error "sys/wait.h:WIFCONTINUED macro is missing from libc-shim"
#endif

#ifndef WIFEXITED
#error "sys/wait.h:WIFEXITED macro is missing from libc-shim"
#endif

#ifndef WIFSIGNALED
#error "sys/wait.h:WIFSIGNALED macro is missing from libc-shim"
#endif

#ifndef WIFSTOPPED
#error "sys/wait.h:WIFSTOPPED macro is missing from libc-shim"
#endif

#ifndef WNOHANG
#error "sys/wait.h:WNOHANG macro is missing from libc-shim"
#endif

#ifndef WNOWAIT
#error "sys/wait.h:WNOWAIT macro is missing from libc-shim"
#endif

#ifndef WSTOPPED
#error "sys/wait.h:WSTOPPED macro is missing from libc-shim"
#endif

#ifndef WSTOPSIG
#error "sys/wait.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "sys/wait.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef WUNTRACED
#error "sys/wait.h:WUNTRACED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
