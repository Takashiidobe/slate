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

#ifndef REG_CS
#error "ucontext.h:REG_CS macro is missing from libc-shim"
#endif

#ifndef REG_DS
#error "ucontext.h:REG_DS macro is missing from libc-shim"
#endif

#ifndef REG_EAX
#error "ucontext.h:REG_EAX macro is missing from libc-shim"
#endif

#ifndef REG_EBP
#error "ucontext.h:REG_EBP macro is missing from libc-shim"
#endif

#ifndef REG_EBX
#error "ucontext.h:REG_EBX macro is missing from libc-shim"
#endif

#ifndef REG_ECX
#error "ucontext.h:REG_ECX macro is missing from libc-shim"
#endif

#ifndef REG_EDI
#error "ucontext.h:REG_EDI macro is missing from libc-shim"
#endif

#ifndef REG_EDX
#error "ucontext.h:REG_EDX macro is missing from libc-shim"
#endif

#ifndef REG_EFL
#error "ucontext.h:REG_EFL macro is missing from libc-shim"
#endif

#ifndef REG_EIP
#error "ucontext.h:REG_EIP macro is missing from libc-shim"
#endif

#ifndef REG_ERR
#error "ucontext.h:REG_ERR macro is missing from libc-shim"
#endif

#ifndef REG_ES
#error "ucontext.h:REG_ES macro is missing from libc-shim"
#endif

#ifndef REG_ESI
#error "ucontext.h:REG_ESI macro is missing from libc-shim"
#endif

#ifndef REG_ESP
#error "ucontext.h:REG_ESP macro is missing from libc-shim"
#endif

#ifndef REG_FS
#error "ucontext.h:REG_FS macro is missing from libc-shim"
#endif

#ifndef REG_GS
#error "ucontext.h:REG_GS macro is missing from libc-shim"
#endif

#ifndef REG_SS
#error "ucontext.h:REG_SS macro is missing from libc-shim"
#endif

#ifndef REG_TRAPNO
#error "ucontext.h:REG_TRAPNO macro is missing from libc-shim"
#endif

#ifndef REG_UESP
#error "ucontext.h:REG_UESP macro is missing from libc-shim"
#endif

int main(void) { return 0; }
