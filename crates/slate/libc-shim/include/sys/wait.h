#ifndef _SLATE_SYS_WAIT_H
#define _SLATE_SYS_WAIT_H

#include <features.h>

#define __NEED_pid_t
#define __NEED_id_t
#include <bits/types.h>

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

int WEXITSTATUS(int s);
int WTERMSIG(int s);
int WSTOPSIG(int s);
int WCOREDUMP(int s);
int WIFEXITED(int s);
int WIFSTOPPED(int s);
int WIFSIGNALED(int s);
int WIFCONTINUED(int s);

#if _REDIR_TIME64
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
__REDIR(wait3, __wait3_time64);
__REDIR(wait4, __wait4_time64);
#endif
#endif

#endif
