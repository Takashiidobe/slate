#include <ucontext.h>

extern int slate_oracle_getcontext(ucontext_t *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getcontext), __typeof__(getcontext)),
    "ucontext.h:getcontext declaration differs from oracle");

static __typeof__(getcontext) *const slate_reference_getcontext = &getcontext;

typedef int slate_oracle_typedef_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_greg_t, greg_t), "typedef greg_t differs from oracle");

#ifndef NGREG
#error "ucontext.h:NGREG macro is missing from libc-shim"
#endif

#ifndef REG_R0
#error "ucontext.h:REG_R0 macro is missing from libc-shim"
#endif

#ifndef REG_R1
#error "ucontext.h:REG_R1 macro is missing from libc-shim"
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

#ifndef REG_R2
#error "ucontext.h:REG_R2 macro is missing from libc-shim"
#endif

#ifndef REG_R3
#error "ucontext.h:REG_R3 macro is missing from libc-shim"
#endif

#ifndef REG_R4
#error "ucontext.h:REG_R4 macro is missing from libc-shim"
#endif

#ifndef REG_R5
#error "ucontext.h:REG_R5 macro is missing from libc-shim"
#endif

#ifndef REG_R6
#error "ucontext.h:REG_R6 macro is missing from libc-shim"
#endif

#ifndef REG_R7
#error "ucontext.h:REG_R7 macro is missing from libc-shim"
#endif

#ifndef REG_R8
#error "ucontext.h:REG_R8 macro is missing from libc-shim"
#endif

#ifndef REG_R9
#error "ucontext.h:REG_R9 macro is missing from libc-shim"
#endif

int main(void) { return 0; }
