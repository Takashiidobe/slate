#ifndef _SLATE_BITS_DARWIN_WAIT_H
#define _SLATE_BITS_DARWIN_WAIT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/wait.h> directly; include a public header instead."
#endif

typedef unsigned int idtype_t;

#define P_ALL  0
#define P_PID  1
#define P_PGID 2

pid_t wait(int *);
pid_t waitpid(pid_t, int *, int);

#include <signal.h>
int waitid(idtype_t, id_t, siginfo_t *, int);

#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#include <sys/resource.h>
pid_t wait3(int *, int, struct rusage *);
pid_t wait4(pid_t, int *, int, struct rusage *);
#endif

#define WNOHANG    0x00000001
#define WUNTRACED  0x00000002
#define WEXITED    0x00000004
#define WSTOPPED   0x00000008
#define WCONTINUED 0x00000010
#define WNOWAIT    0x00000020

#define WCOREFLAG 0200
#define _W_INT(i) (i)
#define _WSTATUS(x) (_W_INT(x) & 0177)
#define _WSTOPPED 0177
#define WIFSTOPPED(x) (_WSTATUS(x) == _WSTOPPED)
#define WSTOPSIG(x) (_W_INT(x) >> 8)
#define WIFSIGNALED(x)                                                        \
  (_WSTATUS(x) != _WSTOPPED && _WSTATUS(x) != 0 && (x) != 0x13)
#define WTERMSIG(x) (_WSTATUS(x))
#define WIFEXITED(x) (_WSTATUS(x) == 0)
#define WEXITSTATUS(x) (_W_INT(x) >> 8)
#define WIFCONTINUED(x) ((x) == 0x13)
#define WCOREDUMP(x) (_W_INT(x) & WCOREFLAG)

#endif
