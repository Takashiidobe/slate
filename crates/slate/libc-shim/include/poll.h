#ifndef _SLATE_POLL_H
#define _SLATE_POLL_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/poll.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/poll.h>

#else

#define POLLIN     0x001
#define POLLPRI    0x002
#define POLLOUT    0x004
#define POLLERR    0x008
#define POLLHUP    0x010
#define POLLNVAL   0x020
#define POLLRDNORM 0x040
#define POLLRDBAND 0x080
#ifndef POLLWRNORM
#define POLLWRNORM 0x100
#define POLLWRBAND 0x200
#endif
#ifndef POLLMSG
#define POLLMSG   0x400
#define POLLRDHUP 0x2000
#endif

typedef unsigned long nfds_t;

struct pollfd {
  int   fd;
  short events;
  short revents;
};

int poll(struct pollfd *, nfds_t, int);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define __NEED_time_t
#define __NEED_struct_timespec
#define __NEED_sigset_t
#include <bits/types.h>
int ppoll(struct pollfd *, nfds_t, const struct timespec *, const sigset_t *);
#endif

#if _REDIR_TIME64
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
__REDIR(ppoll, __ppoll_time64);
#endif
#endif

#endif 

#endif
