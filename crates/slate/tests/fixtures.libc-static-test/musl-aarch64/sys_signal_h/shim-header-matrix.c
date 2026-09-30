#include <sys/signal.h>

typedef unsigned long slate_oracle_typedef_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_greg_t, greg_t), "typedef greg_t differs from oracle");

#ifndef ESR_MAGIC
#error "sys/signal.h:ESR_MAGIC macro is missing from libc-shim"
#endif

#ifndef EXTRA_MAGIC
#error "sys/signal.h:EXTRA_MAGIC macro is missing from libc-shim"
#endif

#ifndef FPSIMD_MAGIC
#error "sys/signal.h:FPSIMD_MAGIC macro is missing from libc-shim"
#endif

#ifndef MINSIGSTKSZ
#error "sys/signal.h:MINSIGSTKSZ macro is missing from libc-shim"
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

#ifndef SVE_MAGIC
#error "sys/signal.h:SVE_MAGIC macro is missing from libc-shim"
#endif

#ifndef SVE_NUM_PREGS
#error "sys/signal.h:SVE_NUM_PREGS macro is missing from libc-shim"
#endif

#ifndef SVE_NUM_ZREGS
#error "sys/signal.h:SVE_NUM_ZREGS macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_CONTEXT_SIZE
#error "sys/signal.h:SVE_SIG_CONTEXT_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_FFR_OFFSET
#error "sys/signal.h:SVE_SIG_FFR_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_FFR_SIZE
#error "sys/signal.h:SVE_SIG_FFR_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREGS_OFFSET
#error "sys/signal.h:SVE_SIG_PREGS_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREGS_SIZE
#error "sys/signal.h:SVE_SIG_PREGS_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREG_OFFSET
#error "sys/signal.h:SVE_SIG_PREG_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREG_SIZE
#error "sys/signal.h:SVE_SIG_PREG_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_REGS_OFFSET
#error "sys/signal.h:SVE_SIG_REGS_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_REGS_SIZE
#error "sys/signal.h:SVE_SIG_REGS_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREGS_OFFSET
#error "sys/signal.h:SVE_SIG_ZREGS_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREGS_SIZE
#error "sys/signal.h:SVE_SIG_ZREGS_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREG_OFFSET
#error "sys/signal.h:SVE_SIG_ZREG_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREG_SIZE
#error "sys/signal.h:SVE_SIG_ZREG_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_VL_MAX
#error "sys/signal.h:SVE_VL_MAX macro is missing from libc-shim"
#endif

#ifndef SVE_VL_MIN
#error "sys/signal.h:SVE_VL_MIN macro is missing from libc-shim"
#endif

#ifndef SVE_VQ_BYTES
#error "sys/signal.h:SVE_VQ_BYTES macro is missing from libc-shim"
#endif

#ifndef SVE_VQ_MAX
#error "sys/signal.h:SVE_VQ_MAX macro is missing from libc-shim"
#endif

#ifndef SVE_VQ_MIN
#error "sys/signal.h:SVE_VQ_MIN macro is missing from libc-shim"
#endif

#ifndef sve_vl_from_vq
#error "sys/signal.h:sve_vl_from_vq macro is missing from libc-shim"
#endif

#ifndef sve_vl_valid
#error "sys/signal.h:sve_vl_valid macro is missing from libc-shim"
#endif

#ifndef sve_vq_from_vl
#error "sys/signal.h:sve_vq_from_vl macro is missing from libc-shim"
#endif

int main(void) { return 0; }
