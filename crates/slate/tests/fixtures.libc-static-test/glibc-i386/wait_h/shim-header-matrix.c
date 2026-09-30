#include <wait.h>

extern int slate_oracle_wait(int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wait), __typeof__(wait)),
    "wait.h:wait declaration differs from oracle");

static __typeof__(wait) *const slate_reference_wait = &wait;

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

#ifndef WAIT_ANY
#error "wait.h:WAIT_ANY macro is missing from libc-shim"
#endif

#ifndef WAIT_MYPGRP
#error "wait.h:WAIT_MYPGRP macro is missing from libc-shim"
#endif

#ifndef WCOREDUMP
#error "wait.h:WCOREDUMP macro is missing from libc-shim"
#endif

#ifndef WCOREFLAG
#error "wait.h:WCOREFLAG macro is missing from libc-shim"
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

#ifndef WSTOPSIG
#error "wait.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "wait.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef W_EXITCODE
#error "wait.h:W_EXITCODE macro is missing from libc-shim"
#endif

#ifndef W_STOPCODE
#error "wait.h:W_STOPCODE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
