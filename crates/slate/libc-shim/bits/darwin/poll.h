#ifndef _SLATE_BITS_DARWIN_POLL_H
#define _SLATE_BITS_DARWIN_POLL_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/poll.h> directly; include a public header instead."
#endif

#define POLLIN     0x0001
#define POLLPRI    0x0002
#define POLLOUT    0x0004
#define POLLERR    0x0008
#define POLLHUP    0x0010
#define POLLNVAL   0x0020
#define POLLRDNORM 0x0040
#define POLLWRNORM POLLOUT
#define POLLRDBAND 0x0080
#define POLLWRBAND 0x0100

typedef unsigned int nfds_t;

struct pollfd {
  int   fd;
  short events;
  short revents;
};

int poll(struct pollfd *, nfds_t, int);

#endif
