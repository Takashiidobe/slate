#ifndef _SLATE_SIGNAL_H
#define _SLATE_SIGNAL_H

typedef int sig_atomic_t;
typedef void (*__slate_sighandler_t)(int);

#define SIG_ERR ((__slate_sighandler_t) - 1)
#define SIG_DFL ((__slate_sighandler_t)0)
#define SIG_IGN ((__slate_sighandler_t)1)

#define SIGHUP 1
#define SIGINT 2
#define SIGILL 4
#define SIGABRT 6
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGTERM 15

__slate_sighandler_t signal(int signum, __slate_sighandler_t handler);
int raise(int sig);

#endif
