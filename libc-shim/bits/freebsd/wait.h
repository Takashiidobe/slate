#ifndef _SLATE_BITS_FREEBSD_WAIT_H
#define _SLATE_BITS_FREEBSD_WAIT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/wait.h> directly; include a public header instead."
#endif

typedef enum {
  P_PID,
  P_PPID,
  P_PGID,
  P_SID,
  P_CID,
  P_UID,
  P_GID,
  P_ALL,
  P_LWPID,
  P_TASKID,
  P_PROJID,
  P_POOLID,
  P_JAILID,
  P_CTID,
  P_CPUID,
  P_PSETID
} idtype_t;

pid_t wait(int *);
pid_t waitpid(pid_t, int *, int);

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#include <signal.h>
int waitid(idtype_t, id_t, siginfo_t *, int);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#include <sys/resource.h>
pid_t wait3(int *, int, struct rusage *);
pid_t wait4(pid_t, int *, int, struct rusage *);
#endif

#define WNOHANG    1
#define WUNTRACED  2
#define WSTOPPED   WUNTRACED
#define WCONTINUED 4
#define WNOWAIT    8
#define WEXITED    16
#define WTRAPPED   32

#define WCOREFLAG 0200

#define _W_INT(i)   (i)
#define _WSTATUS(x) (_W_INT(x) & 0177)
#define _WSTOPPED   0177

#define WIFSTOPPED(x) (_WSTATUS(x) == _WSTOPPED)
#define WSTOPSIG(x)   (_W_INT(x) >> 8)
#define WIFSIGNALED(x)                                                        \
  (_WSTATUS(x) != _WSTOPPED && _WSTATUS(x) != 0 && (x) != 0x13)
#define WTERMSIG(x)     (_WSTATUS(x))
#define WIFEXITED(x)    (_WSTATUS(x) == 0)
#define WEXITSTATUS(x)  (_W_INT(x) >> 8)
#define WIFCONTINUED(x) ((x) == 0x13)
#define WCOREDUMP(x)    (_W_INT(x) & WCOREFLAG)

#endif
