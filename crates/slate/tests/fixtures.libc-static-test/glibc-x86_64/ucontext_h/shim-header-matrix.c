#include <ucontext.h>

extern int slate_oracle_getcontext(ucontext_t *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getcontext), __typeof__(getcontext)),
    "ucontext.h:getcontext declaration differs from oracle");

static __typeof__(getcontext) *const slate_reference_getcontext = &getcontext;

typedef long long slate_oracle_typedef_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_greg_t, greg_t), "typedef greg_t differs from oracle");

#ifndef NGREG
#error "ucontext.h:NGREG macro is missing from libc-shim"
#endif

#ifndef REG_CR2
#error "ucontext.h:REG_CR2 macro is missing from libc-shim"
#endif

#ifndef REG_CSGSFS
#error "ucontext.h:REG_CSGSFS macro is missing from libc-shim"
#endif

#ifndef REG_EFL
#error "ucontext.h:REG_EFL macro is missing from libc-shim"
#endif

#ifndef REG_ERR
#error "ucontext.h:REG_ERR macro is missing from libc-shim"
#endif

#ifndef REG_OLDMASK
#error "ucontext.h:REG_OLDMASK macro is missing from libc-shim"
#endif

#ifndef REG_R10
#error "ucontext.h:REG_R10 macro is missing from libc-shim"
#endif

#ifndef REG_R11
#error "ucontext.h:REG_R11 macro is missing from libc-shim"
#endif

#ifndef REG_R12
#error "ucontext.h:REG_R12 macro is missing from libc-shim"
#endif

#ifndef REG_R13
#error "ucontext.h:REG_R13 macro is missing from libc-shim"
#endif

#ifndef REG_R14
#error "ucontext.h:REG_R14 macro is missing from libc-shim"
#endif

#ifndef REG_R15
#error "ucontext.h:REG_R15 macro is missing from libc-shim"
#endif

#ifndef REG_R8
#error "ucontext.h:REG_R8 macro is missing from libc-shim"
#endif

#ifndef REG_R9
#error "ucontext.h:REG_R9 macro is missing from libc-shim"
#endif

#ifndef REG_RAX
#error "ucontext.h:REG_RAX macro is missing from libc-shim"
#endif

#ifndef REG_RBP
#error "ucontext.h:REG_RBP macro is missing from libc-shim"
#endif

#ifndef REG_RBX
#error "ucontext.h:REG_RBX macro is missing from libc-shim"
#endif

#ifndef REG_RCX
#error "ucontext.h:REG_RCX macro is missing from libc-shim"
#endif

#ifndef REG_RDI
#error "ucontext.h:REG_RDI macro is missing from libc-shim"
#endif

#ifndef REG_RDX
#error "ucontext.h:REG_RDX macro is missing from libc-shim"
#endif

#ifndef REG_RIP
#error "ucontext.h:REG_RIP macro is missing from libc-shim"
#endif

#ifndef REG_RSI
#error "ucontext.h:REG_RSI macro is missing from libc-shim"
#endif

#ifndef REG_RSP
#error "ucontext.h:REG_RSP macro is missing from libc-shim"
#endif

#ifndef REG_TRAPNO
#error "ucontext.h:REG_TRAPNO macro is missing from libc-shim"
#endif

int main(void) { return 0; }
