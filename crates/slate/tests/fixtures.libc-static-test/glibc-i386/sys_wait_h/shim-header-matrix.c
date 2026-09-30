#include <sys/wait.h>

extern int slate_oracle_wait(int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wait), __typeof__(wait)),
    "sys/wait.h:wait declaration differs from oracle");

static __typeof__(wait) *const slate_reference_wait = &wait;

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

#ifndef WAIT_ANY
#error "sys/wait.h:WAIT_ANY macro is missing from libc-shim"
#endif

#ifndef WAIT_MYPGRP
#error "sys/wait.h:WAIT_MYPGRP macro is missing from libc-shim"
#endif

#ifndef WCOREDUMP
#error "sys/wait.h:WCOREDUMP macro is missing from libc-shim"
#endif

#ifndef WCOREFLAG
#error "sys/wait.h:WCOREFLAG macro is missing from libc-shim"
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

#ifndef WSTOPSIG
#error "sys/wait.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "sys/wait.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef W_EXITCODE
#error "sys/wait.h:W_EXITCODE macro is missing from libc-shim"
#endif

#ifndef W_STOPCODE
#error "sys/wait.h:W_STOPCODE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
