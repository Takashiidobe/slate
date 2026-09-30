#include <sys/personality.h>

extern int slate_oracle_personality(unsigned long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_personality), __typeof__(personality)),
    "sys/personality.h:personality declaration differs from oracle");

static __typeof__(personality) *const slate_reference_personality = &personality;

#ifndef ADDR_COMPAT_LAYOUT
#error "sys/personality.h:ADDR_COMPAT_LAYOUT macro is missing from libc-shim"
#endif

#ifndef ADDR_LIMIT_32BIT
#error "sys/personality.h:ADDR_LIMIT_32BIT macro is missing from libc-shim"
#endif

#ifndef ADDR_LIMIT_3GB
#error "sys/personality.h:ADDR_LIMIT_3GB macro is missing from libc-shim"
#endif

#ifndef ADDR_NO_RANDOMIZE
#error "sys/personality.h:ADDR_NO_RANDOMIZE macro is missing from libc-shim"
#endif

#ifndef FDPIC_FUNCPTRS
#error "sys/personality.h:FDPIC_FUNCPTRS macro is missing from libc-shim"
#endif

#ifndef MMAP_PAGE_ZERO
#error "sys/personality.h:MMAP_PAGE_ZERO macro is missing from libc-shim"
#endif

#ifndef PER_BSD
#error "sys/personality.h:PER_BSD macro is missing from libc-shim"
#endif

#ifndef PER_HPUX
#error "sys/personality.h:PER_HPUX macro is missing from libc-shim"
#endif

#ifndef PER_IRIX32
#error "sys/personality.h:PER_IRIX32 macro is missing from libc-shim"
#endif

#ifndef PER_IRIX64
#error "sys/personality.h:PER_IRIX64 macro is missing from libc-shim"
#endif

#ifndef PER_IRIXN32
#error "sys/personality.h:PER_IRIXN32 macro is missing from libc-shim"
#endif

#ifndef PER_ISCR4
#error "sys/personality.h:PER_ISCR4 macro is missing from libc-shim"
#endif

#ifndef PER_LINUX
#error "sys/personality.h:PER_LINUX macro is missing from libc-shim"
#endif

#ifndef PER_LINUX32
#error "sys/personality.h:PER_LINUX32 macro is missing from libc-shim"
#endif

#ifndef PER_LINUX32_3GB
#error "sys/personality.h:PER_LINUX32_3GB macro is missing from libc-shim"
#endif

#ifndef PER_LINUX_32BIT
#error "sys/personality.h:PER_LINUX_32BIT macro is missing from libc-shim"
#endif

#ifndef PER_LINUX_FDPIC
#error "sys/personality.h:PER_LINUX_FDPIC macro is missing from libc-shim"
#endif

#ifndef PER_MASK
#error "sys/personality.h:PER_MASK macro is missing from libc-shim"
#endif

#ifndef PER_OSF4
#error "sys/personality.h:PER_OSF4 macro is missing from libc-shim"
#endif

#ifndef PER_OSR5
#error "sys/personality.h:PER_OSR5 macro is missing from libc-shim"
#endif

#ifndef PER_RISCOS
#error "sys/personality.h:PER_RISCOS macro is missing from libc-shim"
#endif

#ifndef PER_SCOSVR3
#error "sys/personality.h:PER_SCOSVR3 macro is missing from libc-shim"
#endif

#ifndef PER_SOLARIS
#error "sys/personality.h:PER_SOLARIS macro is missing from libc-shim"
#endif

#ifndef PER_SUNOS
#error "sys/personality.h:PER_SUNOS macro is missing from libc-shim"
#endif

#ifndef PER_SVR3
#error "sys/personality.h:PER_SVR3 macro is missing from libc-shim"
#endif

#ifndef PER_SVR4
#error "sys/personality.h:PER_SVR4 macro is missing from libc-shim"
#endif

#ifndef PER_UW7
#error "sys/personality.h:PER_UW7 macro is missing from libc-shim"
#endif

#ifndef PER_WYSEV386
#error "sys/personality.h:PER_WYSEV386 macro is missing from libc-shim"
#endif

#ifndef PER_XENIX
#error "sys/personality.h:PER_XENIX macro is missing from libc-shim"
#endif

#ifndef READ_IMPLIES_EXEC
#error "sys/personality.h:READ_IMPLIES_EXEC macro is missing from libc-shim"
#endif

#ifndef SHORT_INODE
#error "sys/personality.h:SHORT_INODE macro is missing from libc-shim"
#endif

#ifndef STICKY_TIMEOUTS
#error "sys/personality.h:STICKY_TIMEOUTS macro is missing from libc-shim"
#endif

#ifndef UNAME26
#error "sys/personality.h:UNAME26 macro is missing from libc-shim"
#endif

#ifndef WHOLE_SECONDS
#error "sys/personality.h:WHOLE_SECONDS macro is missing from libc-shim"
#endif

int main(void) { return 0; }
