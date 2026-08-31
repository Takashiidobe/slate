#ifndef _SLATE_SYS_WAIT_H
#define _SLATE_SYS_WAIT_H

#include <features.h>

#define __NEED_pid_t
#define __NEED_id_t
#include <bits/types.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/wait.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/wait.h>

#else

typedef enum { P_ALL = 0, P_PID = 1, P_PGID = 2, P_PIDFD = 3 } idtype_t;

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

#define WNOHANG   1
#define WUNTRACED 2

#define WSTOPPED   2
#define WEXITED    4
#define WCONTINUED 8
#define WNOWAIT    0x1000000

#define __WNOTHREAD 0x20000000
#define __WALL      0x40000000
#define __WCLONE    0x80000000

#define WEXITSTATUS(s) (((s) & 0xff00) >> 8)
#define WTERMSIG(s)    ((s) & 0x7f)
#define WSTOPSIG(s)    WEXITSTATUS(s)
#define WCOREDUMP(s)   ((s) & 0x80)
#define WIFEXITED(s)   ((((s) & 0x7f) == 0) ? 1 : 0)
#define WIFSTOPPED(s)                                                          \
  (((short)((((s) & 0xffff) * 0x10001U) >> 8) > 0x7f00) ? 1 : 0)
#define WIFSIGNALED(s)  ((((s) & 0xffff) - 1U < 0xffu) ? 1 : 0)
#define WIFCONTINUED(s) (((s) == 0xffff) ? 1 : 0)

#if _REDIR_TIME64
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
__REDIR(wait3, __wait3_time64);
__REDIR(wait4, __wait4_time64);
#endif
#endif

#endif 

#endif
