#ifndef _SLATE_SYS_TIMERFD_H
#define _SLATE_SYS_TIMERFD_H

#if defined(__SLATE_LIBC_GLIBC)
#define TFD_NONBLOCK 00004000
#define TFD_CLOEXEC  02000000
#else
#include <fcntl.h>
#define TFD_NONBLOCK O_NONBLOCK
#define TFD_CLOEXEC  O_CLOEXEC
#endif
#include <time.h>

#define TFD_TIMER_ABSTIME       1
#define TFD_TIMER_CANCEL_ON_SET (1 << 1)

struct itimerspec;

int timerfd_create(int, int);
int timerfd_settime(int, int, const struct itimerspec *, struct itimerspec *);
int timerfd_gettime(int, struct itimerspec *);

#if _REDIR_TIME64
__REDIR(timerfd_settime, __timerfd_settime64);
__REDIR(timerfd_gettime, __timerfd_gettime64);
#endif

#endif
