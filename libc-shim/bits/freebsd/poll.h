#ifndef _SLATE_BITS_FREEBSD_POLL_H
#define _SLATE_BITS_FREEBSD_POLL_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/poll.h> directly; include a public header instead."
#endif

#define POLLIN       0x0001
#define POLLPRI      0x0002
#define POLLOUT      0x0004
#define POLLRDNORM   0x0040
#define POLLWRNORM   POLLOUT
#define POLLRDBAND   0x0080
#define POLLWRBAND   0x0100
#define POLLINIGNEOF 0x2000
#define POLLRDHUP    0x4000
#define POLLERR      0x0008
#define POLLHUP      0x0010
#define POLLNVAL     0x0020

typedef unsigned int nfds_t;

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

#endif
