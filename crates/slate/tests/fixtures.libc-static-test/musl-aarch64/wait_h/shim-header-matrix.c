#include <wait.h>

extern int slate_oracle_wait3(int *, int, struct rusage *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wait3), __typeof__(wait3)),
    "wait.h:wait3 declaration differs from oracle");

static __typeof__(wait3) *const slate_reference_wait3 = &wait3;

extern int slate_oracle_waitid(idtype_t, unsigned int, siginfo_t *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_waitid), __typeof__(waitid)),
    "wait.h:waitid declaration differs from oracle");

static __typeof__(waitid) *const slate_reference_waitid = &waitid;

#ifndef WCONTINUED
#error "wait.h:WCONTINUED macro is missing from libc-shim"
#endif

#ifndef WCOREDUMP
#error "wait.h:WCOREDUMP macro is missing from libc-shim"
#endif

#ifndef WEXITED
#error "wait.h:WEXITED macro is missing from libc-shim"
#endif

#ifndef WEXITSTATUS
#error "wait.h:WEXITSTATUS macro is missing from libc-shim"
#endif

#ifndef WIFCONTINUED
#error "wait.h:WIFCONTINUED macro is missing from libc-shim"
#endif

#ifndef WIFEXITED
#error "wait.h:WIFEXITED macro is missing from libc-shim"
#endif

#ifndef WIFSIGNALED
#error "wait.h:WIFSIGNALED macro is missing from libc-shim"
#endif

#ifndef WIFSTOPPED
#error "wait.h:WIFSTOPPED macro is missing from libc-shim"
#endif

#ifndef WNOHANG
#error "wait.h:WNOHANG macro is missing from libc-shim"
#endif

#ifndef WNOWAIT
#error "wait.h:WNOWAIT macro is missing from libc-shim"
#endif

#ifndef WSTOPPED
#error "wait.h:WSTOPPED macro is missing from libc-shim"
#endif

#ifndef WSTOPSIG
#error "wait.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "wait.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef WUNTRACED
#error "wait.h:WUNTRACED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
