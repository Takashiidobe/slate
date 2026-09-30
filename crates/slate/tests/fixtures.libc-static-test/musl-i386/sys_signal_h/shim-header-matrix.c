#include <sys/signal.h>

#ifndef MINSIGSTKSZ
#error "sys/signal.h:MINSIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef REG_CS
#error "sys/signal.h:REG_CS macro is missing from libc-shim"
#endif

#ifndef REG_DS
#error "sys/signal.h:REG_DS macro is missing from libc-shim"
#endif

#ifndef REG_EAX
#error "sys/signal.h:REG_EAX macro is missing from libc-shim"
#endif

#ifndef REG_EBP
#error "sys/signal.h:REG_EBP macro is missing from libc-shim"
#endif

#ifndef REG_EBX
#error "sys/signal.h:REG_EBX macro is missing from libc-shim"
#endif

#ifndef REG_ECX
#error "sys/signal.h:REG_ECX macro is missing from libc-shim"
#endif

#ifndef REG_EDI
#error "sys/signal.h:REG_EDI macro is missing from libc-shim"
#endif

#ifndef REG_EDX
#error "sys/signal.h:REG_EDX macro is missing from libc-shim"
#endif

#ifndef REG_EFL
#error "sys/signal.h:REG_EFL macro is missing from libc-shim"
#endif

#ifndef REG_EIP
#error "sys/signal.h:REG_EIP macro is missing from libc-shim"
#endif

#ifndef REG_ERR
#error "sys/signal.h:REG_ERR macro is missing from libc-shim"
#endif

#ifndef REG_ES
#error "sys/signal.h:REG_ES macro is missing from libc-shim"
#endif

#ifndef REG_ESI
#error "sys/signal.h:REG_ESI macro is missing from libc-shim"
#endif

#ifndef REG_ESP
#error "sys/signal.h:REG_ESP macro is missing from libc-shim"
#endif

#ifndef REG_FS
#error "sys/signal.h:REG_FS macro is missing from libc-shim"
#endif

#ifndef REG_GS
#error "sys/signal.h:REG_GS macro is missing from libc-shim"
#endif

#ifndef REG_SS
#error "sys/signal.h:REG_SS macro is missing from libc-shim"
#endif

#ifndef REG_TRAPNO
#error "sys/signal.h:REG_TRAPNO macro is missing from libc-shim"
#endif

#ifndef REG_UESP
#error "sys/signal.h:REG_UESP macro is missing from libc-shim"
#endif

#ifndef SA_NOCLDSTOP
#error "sys/signal.h:SA_NOCLDSTOP macro is missing from libc-shim"
#endif

#ifndef SA_NOCLDWAIT
#error "sys/signal.h:SA_NOCLDWAIT macro is missing from libc-shim"
#endif

#ifndef SA_NODEFER
#error "sys/signal.h:SA_NODEFER macro is missing from libc-shim"
#endif

#ifndef SA_ONSTACK
#error "sys/signal.h:SA_ONSTACK macro is missing from libc-shim"
#endif

#ifndef SA_RESETHAND
#error "sys/signal.h:SA_RESETHAND macro is missing from libc-shim"
#endif

#ifndef SA_RESTART
#error "sys/signal.h:SA_RESTART macro is missing from libc-shim"
#endif

#ifndef SA_RESTORER
#error "sys/signal.h:SA_RESTORER macro is missing from libc-shim"
#endif

#ifndef SA_SIGINFO
#error "sys/signal.h:SA_SIGINFO macro is missing from libc-shim"
#endif

#ifndef SIGABRT
#error "sys/signal.h:SIGABRT macro is missing from libc-shim"
#endif

#ifndef SIGALRM
#error "sys/signal.h:SIGALRM macro is missing from libc-shim"
#endif

#ifndef SIGBUS
#error "sys/signal.h:SIGBUS macro is missing from libc-shim"
#endif

#ifndef SIGCHLD
#error "sys/signal.h:SIGCHLD macro is missing from libc-shim"
#endif

#ifndef SIGCONT
#error "sys/signal.h:SIGCONT macro is missing from libc-shim"
#endif

#ifndef SIGFPE
#error "sys/signal.h:SIGFPE macro is missing from libc-shim"
#endif

#ifndef SIGHUP
#error "sys/signal.h:SIGHUP macro is missing from libc-shim"
#endif

#ifndef SIGILL
#error "sys/signal.h:SIGILL macro is missing from libc-shim"
#endif

#ifndef SIGINT
#error "sys/signal.h:SIGINT macro is missing from libc-shim"
#endif

#ifndef SIGIO
#error "sys/signal.h:SIGIO macro is missing from libc-shim"
#endif

#ifndef SIGIOT
#error "sys/signal.h:SIGIOT macro is missing from libc-shim"
#endif

#ifndef SIGKILL
#error "sys/signal.h:SIGKILL macro is missing from libc-shim"
#endif

#ifndef SIGPIPE
#error "sys/signal.h:SIGPIPE macro is missing from libc-shim"
#endif

#ifndef SIGPOLL
#error "sys/signal.h:SIGPOLL macro is missing from libc-shim"
#endif

#ifndef SIGPROF
#error "sys/signal.h:SIGPROF macro is missing from libc-shim"
#endif

#ifndef SIGPWR
#error "sys/signal.h:SIGPWR macro is missing from libc-shim"
#endif

#ifndef SIGQUIT
#error "sys/signal.h:SIGQUIT macro is missing from libc-shim"
#endif

#ifndef SIGSEGV
#error "sys/signal.h:SIGSEGV macro is missing from libc-shim"
#endif

#ifndef SIGSTKFLT
#error "sys/signal.h:SIGSTKFLT macro is missing from libc-shim"
#endif

#ifndef SIGSTKSZ
#error "sys/signal.h:SIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef SIGSTOP
#error "sys/signal.h:SIGSTOP macro is missing from libc-shim"
#endif

#ifndef SIGSYS
#error "sys/signal.h:SIGSYS macro is missing from libc-shim"
#endif

#ifndef SIGTERM
#error "sys/signal.h:SIGTERM macro is missing from libc-shim"
#endif

#ifndef SIGTRAP
#error "sys/signal.h:SIGTRAP macro is missing from libc-shim"
#endif

#ifndef SIGTSTP
#error "sys/signal.h:SIGTSTP macro is missing from libc-shim"
#endif

#ifndef SIGTTIN
#error "sys/signal.h:SIGTTIN macro is missing from libc-shim"
#endif

#ifndef SIGTTOU
#error "sys/signal.h:SIGTTOU macro is missing from libc-shim"
#endif

#ifndef SIGUNUSED
#error "sys/signal.h:SIGUNUSED macro is missing from libc-shim"
#endif

#ifndef SIGURG
#error "sys/signal.h:SIGURG macro is missing from libc-shim"
#endif

#ifndef SIGUSR1
#error "sys/signal.h:SIGUSR1 macro is missing from libc-shim"
#endif

#ifndef SIGUSR2
#error "sys/signal.h:SIGUSR2 macro is missing from libc-shim"
#endif

#ifndef SIGVTALRM
#error "sys/signal.h:SIGVTALRM macro is missing from libc-shim"
#endif

#ifndef SIGWINCH
#error "sys/signal.h:SIGWINCH macro is missing from libc-shim"
#endif

#ifndef SIGXCPU
#error "sys/signal.h:SIGXCPU macro is missing from libc-shim"
#endif

#ifndef SIGXFSZ
#error "sys/signal.h:SIGXFSZ macro is missing from libc-shim"
#endif

int main(void) { return 0; }
