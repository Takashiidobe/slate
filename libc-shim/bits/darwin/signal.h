#ifndef _SLATE_BITS_DARWIN_SIGNAL_H
#define _SLATE_BITS_DARWIN_SIGNAL_H

#define __NEED_size_t
#define __NEED_pid_t
#define __NEED_uid_t
#define __NEED_pthread_t
#define __NEED_pthread_attr_t
#define __NEED_sigset_t
#include <bits/types.h>

union sigval {
  int   sival_int;
  void *sival_ptr;
};

struct sigevent {
  int             sigev_notify;
  int             sigev_signo;
  union sigval    sigev_value;
  void          (*sigev_notify_function)(union sigval);
  pthread_attr_t *sigev_notify_attributes;
};

typedef struct __siginfo {
  int           si_signo;
  int           si_errno;
  int           si_code;
  pid_t         si_pid;
  uid_t         si_uid;
  int           si_status;
  void         *si_addr;
  union sigval  si_value;
  long          si_band;
  unsigned long __pad[7];
} siginfo_t;

union __sigaction_u {
  void (*__sa_handler)(int);
  void (*__sa_sigaction)(int, siginfo_t *, void *);
};

struct sigaction {
  union __sigaction_u __sigaction_u;
  sigset_t            sa_mask;
  int                 sa_flags;
};

#define sa_handler   __sigaction_u.__sa_handler
#define sa_sigaction __sigaction_u.__sa_sigaction

typedef struct __darwin_sigaltstack {
  void   *ss_sp;
  size_t  ss_size;
  int     ss_flags;
} stack_t;

#define SIGHUP    1
#define SIGINT    2
#define SIGQUIT   3
#define SIGILL    4
#define SIGTRAP   5
#define SIGABRT   6
#define SIGIOT    SIGABRT
#define SIGEMT    7
#define SIGFPE    8
#define SIGKILL   9
#define SIGBUS    10
#define SIGSEGV   11
#define SIGSYS    12
#define SIGPIPE   13
#define SIGALRM   14
#define SIGTERM   15
#define SIGURG    16
#define SIGSTOP   17
#define SIGTSTP   18
#define SIGCONT   19
#define SIGCHLD   20
#define SIGTTIN   21
#define SIGTTOU   22
#define SIGIO     23
#define SIGXCPU   24
#define SIGXFSZ   25
#define SIGVTALRM 26
#define SIGPROF   27
#define SIGWINCH  28
#define SIGINFO   29
#define SIGUSR1   30
#define SIGUSR2   31
#define NSIG      32

#define SIG_DFL  ((void (*)(int))0)
#define SIG_IGN  ((void (*)(int))1)
#define SIG_HOLD ((void (*)(int))5)
#define SIG_ERR  ((void (*)(int))-1)

#define SA_ONSTACK   0x0001
#define SA_RESTART   0x0002
#define SA_RESETHAND 0x0004
#define SA_NOCLDSTOP 0x0008
#define SA_NODEFER   0x0010
#define SA_NOCLDWAIT 0x0020
#define SA_SIGINFO   0x0040

#define SIG_BLOCK   1
#define SIG_UNBLOCK 2
#define SIG_SETMASK 3

#define SI_USER    0x10001
#define SI_QUEUE   0x10002
#define SI_TIMER   0x10003
#define SI_ASYNCIO 0x10004
#define SI_MESGQ   0x10005

#define ILL_ILLOPC 1
#define ILL_ILLTRP 2
#define ILL_PRVOPC 3
#define ILL_ILLOPN 4
#define ILL_ILLADR 5
#define ILL_PRVREG 6
#define ILL_COPROC 7
#define ILL_BADSTK 8

#define FPE_FLTDIV 1
#define FPE_FLTOVF 2
#define FPE_FLTUND 3
#define FPE_FLTRES 4
#define FPE_FLTINV 5
#define FPE_FLTSUB 6
#define FPE_INTDIV 7
#define FPE_INTOVF 8

#define SEGV_MAPERR 1
#define SEGV_ACCERR 2
#define BUS_ADRALN  1
#define BUS_ADRERR  2
#define BUS_OBJERR  3
#define TRAP_BRKPT  1
#define TRAP_TRACE  2

#define CLD_EXITED    1
#define CLD_KILLED    2
#define CLD_DUMPED    3
#define CLD_TRAPPED   4
#define CLD_STOPPED   5
#define CLD_CONTINUED 6

#define POLL_IN  1
#define POLL_OUT 2
#define POLL_MSG 3
#define POLL_ERR 4
#define POLL_PRI 5
#define POLL_HUP 6

#define SS_ONSTACK  0x0001
#define SS_DISABLE  0x0004
#define MINSIGSTKSZ 32768
#define SIGSTKSZ    131072

#define SIGEV_NONE   0
#define SIGEV_SIGNAL 1
#define SIGEV_THREAD 3

typedef int  sig_atomic_t;
typedef void (*sig_t)(int);

void (*signal(int, void (*)(int)))(int);
int raise(int);
void (*bsd_signal(int, void (*)(int)))(int);
int kill(pid_t, int);
int killpg(pid_t, int);
int pthread_kill(pthread_t, int);
int pthread_sigmask(int, const sigset_t *, sigset_t *);
int sigaction(int, const struct sigaction *__restrict,
              struct sigaction *__restrict);
int sigaddset(sigset_t *, int);
int sigaltstack(const stack_t *__restrict, stack_t *__restrict);
int sigdelset(sigset_t *, int);
int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigismember(const sigset_t *, int);
int sigpending(sigset_t *);
int sigprocmask(int, const sigset_t *__restrict, sigset_t *__restrict);
int sigsuspend(const sigset_t *);
int sigwait(const sigset_t *__restrict, int *__restrict);
void psignal(unsigned int, const char *);

#endif
