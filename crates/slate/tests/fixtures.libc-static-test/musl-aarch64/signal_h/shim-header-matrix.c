#include <signal.h>

typedef unsigned long slate_oracle_typedef_greg_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_greg_t, greg_t), "typedef greg_t differs from oracle");

_Static_assert(sizeof(union sigval) == 8, "union sigval size differs from oracle");

_Static_assert(_Alignof(union sigval) == 8, "union sigval alignment differs from oracle");

_Static_assert(__builtin_offsetof(union sigval, sival_int) == 0, "union sigval.sival_int offset differs from oracle");

typedef int slate_oracle_union_sigval_sival_int;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union sigval *)0)->sival_int), slate_oracle_union_sigval_sival_int), "union sigval.sival_int field type differs from oracle");

_Static_assert(__builtin_offsetof(union sigval, sival_ptr) == 0, "union sigval.sival_ptr offset differs from oracle");

typedef void * slate_oracle_union_sigval_sival_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union sigval *)0)->sival_ptr), slate_oracle_union_sigval_sival_ptr), "union sigval.sival_ptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ucontext, uc_flags) == 0, "struct ucontext.uc_flags offset differs from oracle");

typedef unsigned long slate_oracle_struct_ucontext_uc_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ucontext *)0)->uc_flags), slate_oracle_struct_ucontext_uc_flags), "struct ucontext.uc_flags field type differs from oracle");

typedef struct ucontext * slate_oracle_struct_ucontext_uc_link;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ucontext *)0)->uc_link), slate_oracle_struct_ucontext_uc_link), "struct ucontext.uc_link field type differs from oracle");

typedef struct sigaltstack slate_oracle_struct_ucontext_uc_stack;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ucontext *)0)->uc_stack), slate_oracle_struct_ucontext_uc_stack), "struct ucontext.uc_stack field type differs from oracle");

typedef struct __sigset_t slate_oracle_struct_ucontext_uc_sigmask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ucontext *)0)->uc_sigmask), slate_oracle_struct_ucontext_uc_sigmask), "struct ucontext.uc_sigmask field type differs from oracle");

typedef struct sigcontext slate_oracle_struct_ucontext_uc_mcontext;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ucontext *)0)->uc_mcontext), slate_oracle_struct_ucontext_uc_mcontext), "struct ucontext.uc_mcontext field type differs from oracle");

#ifndef BUS_ADRALN
#error "signal.h:BUS_ADRALN macro is missing from libc-shim"
#endif

#ifndef BUS_ADRERR
#error "signal.h:BUS_ADRERR macro is missing from libc-shim"
#endif

#ifndef BUS_MCEERR_AO
#error "signal.h:BUS_MCEERR_AO macro is missing from libc-shim"
#endif

#ifndef BUS_MCEERR_AR
#error "signal.h:BUS_MCEERR_AR macro is missing from libc-shim"
#endif

#ifndef BUS_OBJERR
#error "signal.h:BUS_OBJERR macro is missing from libc-shim"
#endif

#ifndef CLD_CONTINUED
#error "signal.h:CLD_CONTINUED macro is missing from libc-shim"
#endif

#ifndef CLD_DUMPED
#error "signal.h:CLD_DUMPED macro is missing from libc-shim"
#endif

#ifndef CLD_EXITED
#error "signal.h:CLD_EXITED macro is missing from libc-shim"
#endif

#ifndef CLD_KILLED
#error "signal.h:CLD_KILLED macro is missing from libc-shim"
#endif

#ifndef CLD_STOPPED
#error "signal.h:CLD_STOPPED macro is missing from libc-shim"
#endif

#ifndef CLD_TRAPPED
#error "signal.h:CLD_TRAPPED macro is missing from libc-shim"
#endif

#ifndef ESR_MAGIC
#error "signal.h:ESR_MAGIC macro is missing from libc-shim"
#endif

#ifndef EXTRA_MAGIC
#error "signal.h:EXTRA_MAGIC macro is missing from libc-shim"
#endif

#ifndef FPE_FLTDIV
#error "signal.h:FPE_FLTDIV macro is missing from libc-shim"
#endif

#ifndef FPE_FLTINV
#error "signal.h:FPE_FLTINV macro is missing from libc-shim"
#endif

#ifndef FPE_FLTOVF
#error "signal.h:FPE_FLTOVF macro is missing from libc-shim"
#endif

#ifndef FPE_FLTRES
#error "signal.h:FPE_FLTRES macro is missing from libc-shim"
#endif

#ifndef FPE_FLTSUB
#error "signal.h:FPE_FLTSUB macro is missing from libc-shim"
#endif

#ifndef FPE_FLTUND
#error "signal.h:FPE_FLTUND macro is missing from libc-shim"
#endif

#ifndef FPE_INTDIV
#error "signal.h:FPE_INTDIV macro is missing from libc-shim"
#endif

#ifndef FPE_INTOVF
#error "signal.h:FPE_INTOVF macro is missing from libc-shim"
#endif

#ifndef FPSIMD_MAGIC
#error "signal.h:FPSIMD_MAGIC macro is missing from libc-shim"
#endif

#ifndef ILL_BADSTK
#error "signal.h:ILL_BADSTK macro is missing from libc-shim"
#endif

#ifndef ILL_COPROC
#error "signal.h:ILL_COPROC macro is missing from libc-shim"
#endif

#ifndef ILL_ILLADR
#error "signal.h:ILL_ILLADR macro is missing from libc-shim"
#endif

#ifndef ILL_ILLOPC
#error "signal.h:ILL_ILLOPC macro is missing from libc-shim"
#endif

#ifndef ILL_ILLOPN
#error "signal.h:ILL_ILLOPN macro is missing from libc-shim"
#endif

#ifndef ILL_ILLTRP
#error "signal.h:ILL_ILLTRP macro is missing from libc-shim"
#endif

#ifndef ILL_PRVOPC
#error "signal.h:ILL_PRVOPC macro is missing from libc-shim"
#endif

#ifndef ILL_PRVREG
#error "signal.h:ILL_PRVREG macro is missing from libc-shim"
#endif

#ifndef MINSIGSTKSZ
#error "signal.h:MINSIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef NSIG
#error "signal.h:NSIG macro is missing from libc-shim"
#endif

#ifndef POLL_ERR
#error "signal.h:POLL_ERR macro is missing from libc-shim"
#endif

#ifndef POLL_HUP
#error "signal.h:POLL_HUP macro is missing from libc-shim"
#endif

#ifndef POLL_IN
#error "signal.h:POLL_IN macro is missing from libc-shim"
#endif

#ifndef POLL_MSG
#error "signal.h:POLL_MSG macro is missing from libc-shim"
#endif

#ifndef POLL_OUT
#error "signal.h:POLL_OUT macro is missing from libc-shim"
#endif

#ifndef POLL_PRI
#error "signal.h:POLL_PRI macro is missing from libc-shim"
#endif

#ifndef SA_EXPOSE_TAGBITS
#error "signal.h:SA_EXPOSE_TAGBITS macro is missing from libc-shim"
#endif

#ifndef SA_NOCLDSTOP
#error "signal.h:SA_NOCLDSTOP macro is missing from libc-shim"
#endif

#ifndef SA_NOCLDWAIT
#error "signal.h:SA_NOCLDWAIT macro is missing from libc-shim"
#endif

#ifndef SA_NODEFER
#error "signal.h:SA_NODEFER macro is missing from libc-shim"
#endif

#ifndef SA_NOMASK
#error "signal.h:SA_NOMASK macro is missing from libc-shim"
#endif

#ifndef SA_ONESHOT
#error "signal.h:SA_ONESHOT macro is missing from libc-shim"
#endif

#ifndef SA_ONSTACK
#error "signal.h:SA_ONSTACK macro is missing from libc-shim"
#endif

#ifndef SA_RESETHAND
#error "signal.h:SA_RESETHAND macro is missing from libc-shim"
#endif

#ifndef SA_RESTART
#error "signal.h:SA_RESTART macro is missing from libc-shim"
#endif

#ifndef SA_RESTORER
#error "signal.h:SA_RESTORER macro is missing from libc-shim"
#endif

#ifndef SA_SIGINFO
#error "signal.h:SA_SIGINFO macro is missing from libc-shim"
#endif

#ifndef SA_UNSUPPORTED
#error "signal.h:SA_UNSUPPORTED macro is missing from libc-shim"
#endif

#ifndef SEGV_ACCERR
#error "signal.h:SEGV_ACCERR macro is missing from libc-shim"
#endif

#ifndef SEGV_BNDERR
#error "signal.h:SEGV_BNDERR macro is missing from libc-shim"
#endif

#ifndef SEGV_MAPERR
#error "signal.h:SEGV_MAPERR macro is missing from libc-shim"
#endif

#ifndef SEGV_MTEAERR
#error "signal.h:SEGV_MTEAERR macro is missing from libc-shim"
#endif

#ifndef SEGV_MTESERR
#error "signal.h:SEGV_MTESERR macro is missing from libc-shim"
#endif

#ifndef SEGV_PKUERR
#error "signal.h:SEGV_PKUERR macro is missing from libc-shim"
#endif

#ifndef SIGABRT
#error "signal.h:SIGABRT macro is missing from libc-shim"
#endif

#ifndef SIGALRM
#error "signal.h:SIGALRM macro is missing from libc-shim"
#endif

#ifndef SIGBUS
#error "signal.h:SIGBUS macro is missing from libc-shim"
#endif

#ifndef SIGCHLD
#error "signal.h:SIGCHLD macro is missing from libc-shim"
#endif

#ifndef SIGCONT
#error "signal.h:SIGCONT macro is missing from libc-shim"
#endif

#ifndef SIGEV_NONE
#error "signal.h:SIGEV_NONE macro is missing from libc-shim"
#endif

#ifndef SIGEV_SIGNAL
#error "signal.h:SIGEV_SIGNAL macro is missing from libc-shim"
#endif

#ifndef SIGEV_THREAD
#error "signal.h:SIGEV_THREAD macro is missing from libc-shim"
#endif

#ifndef SIGEV_THREAD_ID
#error "signal.h:SIGEV_THREAD_ID macro is missing from libc-shim"
#endif

#ifndef SIGFPE
#error "signal.h:SIGFPE macro is missing from libc-shim"
#endif

#ifndef SIGHUP
#error "signal.h:SIGHUP macro is missing from libc-shim"
#endif

#ifndef SIGILL
#error "signal.h:SIGILL macro is missing from libc-shim"
#endif

#ifndef SIGINT
#error "signal.h:SIGINT macro is missing from libc-shim"
#endif

#ifndef SIGIO
#error "signal.h:SIGIO macro is missing from libc-shim"
#endif

#ifndef SIGIOT
#error "signal.h:SIGIOT macro is missing from libc-shim"
#endif

#ifndef SIGKILL
#error "signal.h:SIGKILL macro is missing from libc-shim"
#endif

#ifndef SIGPIPE
#error "signal.h:SIGPIPE macro is missing from libc-shim"
#endif

#ifndef SIGPOLL
#error "signal.h:SIGPOLL macro is missing from libc-shim"
#endif

#ifndef SIGPROF
#error "signal.h:SIGPROF macro is missing from libc-shim"
#endif

#ifndef SIGPWR
#error "signal.h:SIGPWR macro is missing from libc-shim"
#endif

#ifndef SIGQUIT
#error "signal.h:SIGQUIT macro is missing from libc-shim"
#endif

#ifndef SIGRTMAX
#error "signal.h:SIGRTMAX macro is missing from libc-shim"
#endif

#ifndef SIGRTMIN
#error "signal.h:SIGRTMIN macro is missing from libc-shim"
#endif

#ifndef SIGSEGV
#error "signal.h:SIGSEGV macro is missing from libc-shim"
#endif

#ifndef SIGSTKFLT
#error "signal.h:SIGSTKFLT macro is missing from libc-shim"
#endif

#ifndef SIGSTKSZ
#error "signal.h:SIGSTKSZ macro is missing from libc-shim"
#endif

#ifndef SIGSTOP
#error "signal.h:SIGSTOP macro is missing from libc-shim"
#endif

#ifndef SIGSYS
#error "signal.h:SIGSYS macro is missing from libc-shim"
#endif

#ifndef SIGTERM
#error "signal.h:SIGTERM macro is missing from libc-shim"
#endif

#ifndef SIGTRAP
#error "signal.h:SIGTRAP macro is missing from libc-shim"
#endif

#ifndef SIGTSTP
#error "signal.h:SIGTSTP macro is missing from libc-shim"
#endif

#ifndef SIGTTIN
#error "signal.h:SIGTTIN macro is missing from libc-shim"
#endif

#ifndef SIGTTOU
#error "signal.h:SIGTTOU macro is missing from libc-shim"
#endif

#ifndef SIGUNUSED
#error "signal.h:SIGUNUSED macro is missing from libc-shim"
#endif

#ifndef SIGURG
#error "signal.h:SIGURG macro is missing from libc-shim"
#endif

#ifndef SIGUSR1
#error "signal.h:SIGUSR1 macro is missing from libc-shim"
#endif

#ifndef SIGUSR2
#error "signal.h:SIGUSR2 macro is missing from libc-shim"
#endif

#ifndef SIGVTALRM
#error "signal.h:SIGVTALRM macro is missing from libc-shim"
#endif

#ifndef SIGWINCH
#error "signal.h:SIGWINCH macro is missing from libc-shim"
#endif

#ifndef SIGXCPU
#error "signal.h:SIGXCPU macro is missing from libc-shim"
#endif

#ifndef SIGXFSZ
#error "signal.h:SIGXFSZ macro is missing from libc-shim"
#endif

#ifndef SIG_BLOCK
#error "signal.h:SIG_BLOCK macro is missing from libc-shim"
#endif

#ifndef SIG_DFL
#error "signal.h:SIG_DFL macro is missing from libc-shim"
#endif

#ifndef SIG_ERR
#error "signal.h:SIG_ERR macro is missing from libc-shim"
#endif

#ifndef SIG_HOLD
#error "signal.h:SIG_HOLD macro is missing from libc-shim"
#endif

#ifndef SIG_IGN
#error "signal.h:SIG_IGN macro is missing from libc-shim"
#endif

#ifndef SIG_SETMASK
#error "signal.h:SIG_SETMASK macro is missing from libc-shim"
#endif

#ifndef SIG_UNBLOCK
#error "signal.h:SIG_UNBLOCK macro is missing from libc-shim"
#endif

#ifndef SI_ASYNCIO
#error "signal.h:SI_ASYNCIO macro is missing from libc-shim"
#endif

#ifndef SI_ASYNCNL
#error "signal.h:SI_ASYNCNL macro is missing from libc-shim"
#endif

#ifndef SI_KERNEL
#error "signal.h:SI_KERNEL macro is missing from libc-shim"
#endif

#ifndef SI_MESGQ
#error "signal.h:SI_MESGQ macro is missing from libc-shim"
#endif

#ifndef SI_QUEUE
#error "signal.h:SI_QUEUE macro is missing from libc-shim"
#endif

#ifndef SI_SIGIO
#error "signal.h:SI_SIGIO macro is missing from libc-shim"
#endif

#ifndef SI_TIMER
#error "signal.h:SI_TIMER macro is missing from libc-shim"
#endif

#ifndef SI_TKILL
#error "signal.h:SI_TKILL macro is missing from libc-shim"
#endif

#ifndef SI_USER
#error "signal.h:SI_USER macro is missing from libc-shim"
#endif

#ifndef SS_AUTODISARM
#error "signal.h:SS_AUTODISARM macro is missing from libc-shim"
#endif

#ifndef SS_DISABLE
#error "signal.h:SS_DISABLE macro is missing from libc-shim"
#endif

#ifndef SS_FLAG_BITS
#error "signal.h:SS_FLAG_BITS macro is missing from libc-shim"
#endif

#ifndef SS_ONSTACK
#error "signal.h:SS_ONSTACK macro is missing from libc-shim"
#endif

#ifndef SVE_MAGIC
#error "signal.h:SVE_MAGIC macro is missing from libc-shim"
#endif

#ifndef SVE_NUM_PREGS
#error "signal.h:SVE_NUM_PREGS macro is missing from libc-shim"
#endif

#ifndef SVE_NUM_ZREGS
#error "signal.h:SVE_NUM_ZREGS macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_CONTEXT_SIZE
#error "signal.h:SVE_SIG_CONTEXT_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_FFR_OFFSET
#error "signal.h:SVE_SIG_FFR_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_FFR_SIZE
#error "signal.h:SVE_SIG_FFR_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREGS_OFFSET
#error "signal.h:SVE_SIG_PREGS_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREGS_SIZE
#error "signal.h:SVE_SIG_PREGS_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREG_OFFSET
#error "signal.h:SVE_SIG_PREG_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_PREG_SIZE
#error "signal.h:SVE_SIG_PREG_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_REGS_OFFSET
#error "signal.h:SVE_SIG_REGS_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_REGS_SIZE
#error "signal.h:SVE_SIG_REGS_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREGS_OFFSET
#error "signal.h:SVE_SIG_ZREGS_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREGS_SIZE
#error "signal.h:SVE_SIG_ZREGS_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREG_OFFSET
#error "signal.h:SVE_SIG_ZREG_OFFSET macro is missing from libc-shim"
#endif

#ifndef SVE_SIG_ZREG_SIZE
#error "signal.h:SVE_SIG_ZREG_SIZE macro is missing from libc-shim"
#endif

#ifndef SVE_VL_MAX
#error "signal.h:SVE_VL_MAX macro is missing from libc-shim"
#endif

#ifndef SVE_VL_MIN
#error "signal.h:SVE_VL_MIN macro is missing from libc-shim"
#endif

#ifndef SVE_VQ_BYTES
#error "signal.h:SVE_VQ_BYTES macro is missing from libc-shim"
#endif

#ifndef SVE_VQ_MAX
#error "signal.h:SVE_VQ_MAX macro is missing from libc-shim"
#endif

#ifndef SVE_VQ_MIN
#error "signal.h:SVE_VQ_MIN macro is missing from libc-shim"
#endif

#ifndef SYS_SECCOMP
#error "signal.h:SYS_SECCOMP macro is missing from libc-shim"
#endif

#ifndef SYS_USER_DISPATCH
#error "signal.h:SYS_USER_DISPATCH macro is missing from libc-shim"
#endif

#ifndef TRAP_BRANCH
#error "signal.h:TRAP_BRANCH macro is missing from libc-shim"
#endif

#ifndef TRAP_BRKPT
#error "signal.h:TRAP_BRKPT macro is missing from libc-shim"
#endif

#ifndef TRAP_HWBKPT
#error "signal.h:TRAP_HWBKPT macro is missing from libc-shim"
#endif

#ifndef TRAP_TRACE
#error "signal.h:TRAP_TRACE macro is missing from libc-shim"
#endif

#ifndef TRAP_UNK
#error "signal.h:TRAP_UNK macro is missing from libc-shim"
#endif

#ifndef sa_handler
#error "signal.h:sa_handler macro is missing from libc-shim"
#endif

#ifndef sa_sigaction
#error "signal.h:sa_sigaction macro is missing from libc-shim"
#endif

#ifndef si_addr
#error "signal.h:si_addr macro is missing from libc-shim"
#endif

#ifndef si_addr_lsb
#error "signal.h:si_addr_lsb macro is missing from libc-shim"
#endif

#ifndef si_arch
#error "signal.h:si_arch macro is missing from libc-shim"
#endif

#ifndef si_band
#error "signal.h:si_band macro is missing from libc-shim"
#endif

#ifndef si_call_addr
#error "signal.h:si_call_addr macro is missing from libc-shim"
#endif

#ifndef si_fd
#error "signal.h:si_fd macro is missing from libc-shim"
#endif

#ifndef si_int
#error "signal.h:si_int macro is missing from libc-shim"
#endif

#ifndef si_lower
#error "signal.h:si_lower macro is missing from libc-shim"
#endif

#ifndef si_overrun
#error "signal.h:si_overrun macro is missing from libc-shim"
#endif

#ifndef si_pid
#error "signal.h:si_pid macro is missing from libc-shim"
#endif

#ifndef si_pkey
#error "signal.h:si_pkey macro is missing from libc-shim"
#endif

#ifndef si_ptr
#error "signal.h:si_ptr macro is missing from libc-shim"
#endif

#ifndef si_status
#error "signal.h:si_status macro is missing from libc-shim"
#endif

#ifndef si_stime
#error "signal.h:si_stime macro is missing from libc-shim"
#endif

#ifndef si_syscall
#error "signal.h:si_syscall macro is missing from libc-shim"
#endif

#ifndef si_timerid
#error "signal.h:si_timerid macro is missing from libc-shim"
#endif

#ifndef si_uid
#error "signal.h:si_uid macro is missing from libc-shim"
#endif

#ifndef si_upper
#error "signal.h:si_upper macro is missing from libc-shim"
#endif

#ifndef si_utime
#error "signal.h:si_utime macro is missing from libc-shim"
#endif

#ifndef si_value
#error "signal.h:si_value macro is missing from libc-shim"
#endif

#ifndef sigev_notify_attributes
#error "signal.h:sigev_notify_attributes macro is missing from libc-shim"
#endif

#ifndef sigev_notify_function
#error "signal.h:sigev_notify_function macro is missing from libc-shim"
#endif

#ifndef sigev_notify_thread_id
#error "signal.h:sigev_notify_thread_id macro is missing from libc-shim"
#endif

#ifndef sve_vl_from_vq
#error "signal.h:sve_vl_from_vq macro is missing from libc-shim"
#endif

#ifndef sve_vl_valid
#error "signal.h:sve_vl_valid macro is missing from libc-shim"
#endif

#ifndef sve_vq_from_vl
#error "signal.h:sve_vq_from_vl macro is missing from libc-shim"
#endif

int main(void) { return 0; }
