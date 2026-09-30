#include <signal.h>

extern int slate_oracle_sigaltstack(const stack_t *restrict, stack_t *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sigaltstack), __typeof__(sigaltstack)),
    "signal.h:sigaltstack declaration differs from oracle");

static __typeof__(sigaltstack) *const slate_reference_sigaltstack = &sigaltstack;

extern int slate_oracle_siginterrupt(int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_siginterrupt), __typeof__(siginterrupt)),
    "signal.h:siginterrupt declaration differs from oracle");

static __typeof__(siginterrupt) *const slate_reference_siginterrupt = &siginterrupt;

extern int slate_oracle_sigprocmask(int, const struct __sigset_t *restrict, struct __sigset_t *restrict);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sigprocmask), __typeof__(sigprocmask)),
    "signal.h:sigprocmask declaration differs from oracle");

static __typeof__(sigprocmask) *const slate_reference_sigprocmask = &sigprocmask;

extern int slate_oracle_sigreturn(struct sigcontext *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sigreturn), __typeof__(sigreturn)),
    "signal.h:sigreturn declaration differs from oracle");

static __typeof__(sigreturn) *const slate_reference_sigreturn = &sigreturn;

extern int slate_oracle_sigstack(struct sigstack *, struct sigstack *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sigstack), __typeof__(sigstack)),
    "signal.h:sigstack declaration differs from oracle");

static __typeof__(sigstack) *const slate_reference_sigstack = &sigstack;

extern int slate_oracle_tgkill(int, int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgkill), __typeof__(tgkill)),
    "signal.h:tgkill declaration differs from oracle");

static __typeof__(tgkill) *const slate_reference_tgkill = &tgkill;

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

#ifndef NSIG
#error "signal.h:NSIG macro is missing from libc-shim"
#endif

#ifndef SIGRTMAX
#error "signal.h:SIGRTMAX macro is missing from libc-shim"
#endif

#ifndef SIGRTMIN
#error "signal.h:SIGRTMIN macro is missing from libc-shim"
#endif

#ifndef sigmask
#error "signal.h:sigmask macro is missing from libc-shim"
#endif

int main(void) { return 0; }
