#include <signal.h>

extern void ** slate_oracle___pxcptinfoptrs(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___pxcptinfoptrs), __typeof__(__pxcptinfoptrs)),
    "signal.h:__pxcptinfoptrs declaration differs from oracle");

static __typeof__(__pxcptinfoptrs) *const slate_reference___pxcptinfoptrs = &__pxcptinfoptrs;

extern int slate_oracle_raise(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_raise), __typeof__(raise)),
    "signal.h:raise declaration differs from oracle");

static __typeof__(raise) *const slate_reference_raise = &raise;

extern void slate_oracle_signal(*)(int) __attribute__((cdecl)) (int, void (*)(int) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_signal), __typeof__(signal)),
    "signal.h:signal declaration differs from oracle");

static __typeof__(signal) *const slate_reference_signal = &signal;

typedef int slate_oracle_typedef_sig_atomic_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sig_atomic_t, sig_atomic_t), "typedef sig_atomic_t differs from oracle");

#ifndef NSIG
#error "signal.h:NSIG macro is missing from libc-shim"
#endif

#ifndef SIGABRT
#error "signal.h:SIGABRT macro is missing from libc-shim"
#endif

#ifndef SIGABRT_COMPAT
#error "signal.h:SIGABRT_COMPAT macro is missing from libc-shim"
#endif

#ifndef SIGBREAK
#error "signal.h:SIGBREAK macro is missing from libc-shim"
#endif

#ifndef SIGFPE
#error "signal.h:SIGFPE macro is missing from libc-shim"
#endif

#ifndef SIGILL
#error "signal.h:SIGILL macro is missing from libc-shim"
#endif

#ifndef SIGINT
#error "signal.h:SIGINT macro is missing from libc-shim"
#endif

#ifndef SIGSEGV
#error "signal.h:SIGSEGV macro is missing from libc-shim"
#endif

#ifndef SIGTERM
#error "signal.h:SIGTERM macro is missing from libc-shim"
#endif

#ifndef SIG_ACK
#error "signal.h:SIG_ACK macro is missing from libc-shim"
#endif

#ifndef SIG_DFL
#error "signal.h:SIG_DFL macro is missing from libc-shim"
#endif

#ifndef SIG_ERR
#error "signal.h:SIG_ERR macro is missing from libc-shim"
#endif

#ifndef SIG_GET
#error "signal.h:SIG_GET macro is missing from libc-shim"
#endif

#ifndef SIG_IGN
#error "signal.h:SIG_IGN macro is missing from libc-shim"
#endif

#ifndef SIG_SGE
#error "signal.h:SIG_SGE macro is missing from libc-shim"
#endif

#ifndef _INC_SIGNAL
#error "signal.h:_INC_SIGNAL macro is missing from libc-shim"
#endif

#ifndef _pxcptinfoptrs
#error "signal.h:_pxcptinfoptrs macro is missing from libc-shim"
#endif

int main(void) { return 0; }
