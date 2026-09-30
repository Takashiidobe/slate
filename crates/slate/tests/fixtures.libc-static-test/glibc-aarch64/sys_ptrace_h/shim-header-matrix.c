#include <sys/ptrace.h>

#ifndef PTRACE_GETREGSET
#error "sys/ptrace.h:PTRACE_GETREGSET macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETSIGMASK
#error "sys/ptrace.h:PTRACE_GETSIGMASK macro is missing from libc-shim"
#endif

#ifndef PTRACE_GET_RSEQ_CONFIGURATION
#error "sys/ptrace.h:PTRACE_GET_RSEQ_CONFIGURATION macro is missing from libc-shim"
#endif

#ifndef PTRACE_GET_SYSCALL_INFO
#error "sys/ptrace.h:PTRACE_GET_SYSCALL_INFO macro is missing from libc-shim"
#endif

#ifndef PTRACE_GET_SYSCALL_USER_DISPATCH_CONFIG
#error "sys/ptrace.h:PTRACE_GET_SYSCALL_USER_DISPATCH_CONFIG macro is missing from libc-shim"
#endif

#ifndef PTRACE_INTERRUPT
#error "sys/ptrace.h:PTRACE_INTERRUPT macro is missing from libc-shim"
#endif

#ifndef PTRACE_LISTEN
#error "sys/ptrace.h:PTRACE_LISTEN macro is missing from libc-shim"
#endif

#ifndef PTRACE_PEEKSIGINFO
#error "sys/ptrace.h:PTRACE_PEEKSIGINFO macro is missing from libc-shim"
#endif

#ifndef PTRACE_SECCOMP_GET_FILTER
#error "sys/ptrace.h:PTRACE_SECCOMP_GET_FILTER macro is missing from libc-shim"
#endif

#ifndef PTRACE_SECCOMP_GET_METADATA
#error "sys/ptrace.h:PTRACE_SECCOMP_GET_METADATA macro is missing from libc-shim"
#endif

#ifndef PTRACE_SEIZE
#error "sys/ptrace.h:PTRACE_SEIZE macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETREGSET
#error "sys/ptrace.h:PTRACE_SETREGSET macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETSIGMASK
#error "sys/ptrace.h:PTRACE_SETSIGMASK macro is missing from libc-shim"
#endif

#ifndef PTRACE_SET_SYSCALL_USER_DISPATCH_CONFIG
#error "sys/ptrace.h:PTRACE_SET_SYSCALL_USER_DISPATCH_CONFIG macro is missing from libc-shim"
#endif

#ifndef PT_ATTACH
#error "sys/ptrace.h:PT_ATTACH macro is missing from libc-shim"
#endif

#ifndef PT_CONTINUE
#error "sys/ptrace.h:PT_CONTINUE macro is missing from libc-shim"
#endif

#ifndef PT_DETACH
#error "sys/ptrace.h:PT_DETACH macro is missing from libc-shim"
#endif

#ifndef PT_GETEVENTMSG
#error "sys/ptrace.h:PT_GETEVENTMSG macro is missing from libc-shim"
#endif

#ifndef PT_GETSIGINFO
#error "sys/ptrace.h:PT_GETSIGINFO macro is missing from libc-shim"
#endif

#ifndef PT_KILL
#error "sys/ptrace.h:PT_KILL macro is missing from libc-shim"
#endif

#ifndef PT_PEEKMTETAGS
#error "sys/ptrace.h:PT_PEEKMTETAGS macro is missing from libc-shim"
#endif

#ifndef PT_POKEMTETAGS
#error "sys/ptrace.h:PT_POKEMTETAGS macro is missing from libc-shim"
#endif

#ifndef PT_READ_D
#error "sys/ptrace.h:PT_READ_D macro is missing from libc-shim"
#endif

#ifndef PT_READ_I
#error "sys/ptrace.h:PT_READ_I macro is missing from libc-shim"
#endif

#ifndef PT_READ_U
#error "sys/ptrace.h:PT_READ_U macro is missing from libc-shim"
#endif

#ifndef PT_SETOPTIONS
#error "sys/ptrace.h:PT_SETOPTIONS macro is missing from libc-shim"
#endif

#ifndef PT_SETSIGINFO
#error "sys/ptrace.h:PT_SETSIGINFO macro is missing from libc-shim"
#endif

#ifndef PT_STEP
#error "sys/ptrace.h:PT_STEP macro is missing from libc-shim"
#endif

#ifndef PT_SYSCALL
#error "sys/ptrace.h:PT_SYSCALL macro is missing from libc-shim"
#endif

#ifndef PT_SYSEMU
#error "sys/ptrace.h:PT_SYSEMU macro is missing from libc-shim"
#endif

#ifndef PT_SYSEMU_SINGLESTEP
#error "sys/ptrace.h:PT_SYSEMU_SINGLESTEP macro is missing from libc-shim"
#endif

#ifndef PT_TRACE_ME
#error "sys/ptrace.h:PT_TRACE_ME macro is missing from libc-shim"
#endif

#ifndef PT_WRITE_D
#error "sys/ptrace.h:PT_WRITE_D macro is missing from libc-shim"
#endif

#ifndef PT_WRITE_I
#error "sys/ptrace.h:PT_WRITE_I macro is missing from libc-shim"
#endif

#ifndef PT_WRITE_U
#error "sys/ptrace.h:PT_WRITE_U macro is missing from libc-shim"
#endif

int main(void) { return 0; }
