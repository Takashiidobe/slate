#include <sys/ptrace.h>

#ifndef PTRACE_ATTACH
#error "sys/ptrace.h:PTRACE_ATTACH macro is missing from libc-shim"
#endif

#ifndef PTRACE_CONT
#error "sys/ptrace.h:PTRACE_CONT macro is missing from libc-shim"
#endif

#ifndef PTRACE_DETACH
#error "sys/ptrace.h:PTRACE_DETACH macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_CLONE
#error "sys/ptrace.h:PTRACE_EVENT_CLONE macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_EXEC
#error "sys/ptrace.h:PTRACE_EVENT_EXEC macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_EXIT
#error "sys/ptrace.h:PTRACE_EVENT_EXIT macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_FORK
#error "sys/ptrace.h:PTRACE_EVENT_FORK macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_SECCOMP
#error "sys/ptrace.h:PTRACE_EVENT_SECCOMP macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_STOP
#error "sys/ptrace.h:PTRACE_EVENT_STOP macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_VFORK
#error "sys/ptrace.h:PTRACE_EVENT_VFORK macro is missing from libc-shim"
#endif

#ifndef PTRACE_EVENT_VFORK_DONE
#error "sys/ptrace.h:PTRACE_EVENT_VFORK_DONE macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETEVENTMSG
#error "sys/ptrace.h:PTRACE_GETEVENTMSG macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETFPREGS
#error "sys/ptrace.h:PTRACE_GETFPREGS macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETFPXREGS
#error "sys/ptrace.h:PTRACE_GETFPXREGS macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETREGS
#error "sys/ptrace.h:PTRACE_GETREGS macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETREGSET
#error "sys/ptrace.h:PTRACE_GETREGSET macro is missing from libc-shim"
#endif

#ifndef PTRACE_GETSIGINFO
#error "sys/ptrace.h:PTRACE_GETSIGINFO macro is missing from libc-shim"
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

#ifndef PTRACE_GET_THREAD_AREA
#error "sys/ptrace.h:PTRACE_GET_THREAD_AREA macro is missing from libc-shim"
#endif

#ifndef PTRACE_INTERRUPT
#error "sys/ptrace.h:PTRACE_INTERRUPT macro is missing from libc-shim"
#endif

#ifndef PTRACE_KILL
#error "sys/ptrace.h:PTRACE_KILL macro is missing from libc-shim"
#endif

#ifndef PTRACE_LISTEN
#error "sys/ptrace.h:PTRACE_LISTEN macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_EXITKILL
#error "sys/ptrace.h:PTRACE_O_EXITKILL macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_MASK
#error "sys/ptrace.h:PTRACE_O_MASK macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_SUSPEND_SECCOMP
#error "sys/ptrace.h:PTRACE_O_SUSPEND_SECCOMP macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACECLONE
#error "sys/ptrace.h:PTRACE_O_TRACECLONE macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACEEXEC
#error "sys/ptrace.h:PTRACE_O_TRACEEXEC macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACEEXIT
#error "sys/ptrace.h:PTRACE_O_TRACEEXIT macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACEFORK
#error "sys/ptrace.h:PTRACE_O_TRACEFORK macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACESECCOMP
#error "sys/ptrace.h:PTRACE_O_TRACESECCOMP macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACESYSGOOD
#error "sys/ptrace.h:PTRACE_O_TRACESYSGOOD macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACEVFORK
#error "sys/ptrace.h:PTRACE_O_TRACEVFORK macro is missing from libc-shim"
#endif

#ifndef PTRACE_O_TRACEVFORKDONE
#error "sys/ptrace.h:PTRACE_O_TRACEVFORKDONE macro is missing from libc-shim"
#endif

#ifndef PTRACE_PEEKDATA
#error "sys/ptrace.h:PTRACE_PEEKDATA macro is missing from libc-shim"
#endif

#ifndef PTRACE_PEEKSIGINFO
#error "sys/ptrace.h:PTRACE_PEEKSIGINFO macro is missing from libc-shim"
#endif

#ifndef PTRACE_PEEKSIGINFO_SHARED
#error "sys/ptrace.h:PTRACE_PEEKSIGINFO_SHARED macro is missing from libc-shim"
#endif

#ifndef PTRACE_PEEKTEXT
#error "sys/ptrace.h:PTRACE_PEEKTEXT macro is missing from libc-shim"
#endif

#ifndef PTRACE_PEEKUSER
#error "sys/ptrace.h:PTRACE_PEEKUSER macro is missing from libc-shim"
#endif

#ifndef PTRACE_POKEDATA
#error "sys/ptrace.h:PTRACE_POKEDATA macro is missing from libc-shim"
#endif

#ifndef PTRACE_POKETEXT
#error "sys/ptrace.h:PTRACE_POKETEXT macro is missing from libc-shim"
#endif

#ifndef PTRACE_POKEUSER
#error "sys/ptrace.h:PTRACE_POKEUSER macro is missing from libc-shim"
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

#ifndef PTRACE_SETFPREGS
#error "sys/ptrace.h:PTRACE_SETFPREGS macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETFPXREGS
#error "sys/ptrace.h:PTRACE_SETFPXREGS macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETOPTIONS
#error "sys/ptrace.h:PTRACE_SETOPTIONS macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETREGS
#error "sys/ptrace.h:PTRACE_SETREGS macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETREGSET
#error "sys/ptrace.h:PTRACE_SETREGSET macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETSIGINFO
#error "sys/ptrace.h:PTRACE_SETSIGINFO macro is missing from libc-shim"
#endif

#ifndef PTRACE_SETSIGMASK
#error "sys/ptrace.h:PTRACE_SETSIGMASK macro is missing from libc-shim"
#endif

#ifndef PTRACE_SET_THREAD_AREA
#error "sys/ptrace.h:PTRACE_SET_THREAD_AREA macro is missing from libc-shim"
#endif

#ifndef PTRACE_SINGLEBLOCK
#error "sys/ptrace.h:PTRACE_SINGLEBLOCK macro is missing from libc-shim"
#endif

#ifndef PTRACE_SINGLESTEP
#error "sys/ptrace.h:PTRACE_SINGLESTEP macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSCALL
#error "sys/ptrace.h:PTRACE_SYSCALL macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSCALL_INFO_ENTRY
#error "sys/ptrace.h:PTRACE_SYSCALL_INFO_ENTRY macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSCALL_INFO_EXIT
#error "sys/ptrace.h:PTRACE_SYSCALL_INFO_EXIT macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSCALL_INFO_NONE
#error "sys/ptrace.h:PTRACE_SYSCALL_INFO_NONE macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSCALL_INFO_SECCOMP
#error "sys/ptrace.h:PTRACE_SYSCALL_INFO_SECCOMP macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSEMU
#error "sys/ptrace.h:PTRACE_SYSEMU macro is missing from libc-shim"
#endif

#ifndef PTRACE_SYSEMU_SINGLESTEP
#error "sys/ptrace.h:PTRACE_SYSEMU_SINGLESTEP macro is missing from libc-shim"
#endif

#ifndef PTRACE_TRACEME
#error "sys/ptrace.h:PTRACE_TRACEME macro is missing from libc-shim"
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

#ifndef PT_GETFPREGS
#error "sys/ptrace.h:PT_GETFPREGS macro is missing from libc-shim"
#endif

#ifndef PT_GETFPXREGS
#error "sys/ptrace.h:PT_GETFPXREGS macro is missing from libc-shim"
#endif

#ifndef PT_GETREGS
#error "sys/ptrace.h:PT_GETREGS macro is missing from libc-shim"
#endif

#ifndef PT_GETSIGINFO
#error "sys/ptrace.h:PT_GETSIGINFO macro is missing from libc-shim"
#endif

#ifndef PT_GET_THREAD_AREA
#error "sys/ptrace.h:PT_GET_THREAD_AREA macro is missing from libc-shim"
#endif

#ifndef PT_KILL
#error "sys/ptrace.h:PT_KILL macro is missing from libc-shim"
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

#ifndef PT_SETFPREGS
#error "sys/ptrace.h:PT_SETFPREGS macro is missing from libc-shim"
#endif

#ifndef PT_SETFPXREGS
#error "sys/ptrace.h:PT_SETFPXREGS macro is missing from libc-shim"
#endif

#ifndef PT_SETOPTIONS
#error "sys/ptrace.h:PT_SETOPTIONS macro is missing from libc-shim"
#endif

#ifndef PT_SETREGS
#error "sys/ptrace.h:PT_SETREGS macro is missing from libc-shim"
#endif

#ifndef PT_SETSIGINFO
#error "sys/ptrace.h:PT_SETSIGINFO macro is missing from libc-shim"
#endif

#ifndef PT_SET_THREAD_AREA
#error "sys/ptrace.h:PT_SET_THREAD_AREA macro is missing from libc-shim"
#endif

#ifndef PT_STEP
#error "sys/ptrace.h:PT_STEP macro is missing from libc-shim"
#endif

#ifndef PT_STEPBLOCK
#error "sys/ptrace.h:PT_STEPBLOCK macro is missing from libc-shim"
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
