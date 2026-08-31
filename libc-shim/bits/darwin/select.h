#ifndef _SLATE_BITS_DARWIN_SELECT_H
#define _SLATE_BITS_DARWIN_SELECT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/select.h> directly; include a public header instead."
#endif

#define FD_SETSIZE 1024
#define NFDBITS    32

typedef int fd_mask;

typedef struct fd_set {
  fd_mask fds_bits[FD_SETSIZE / NFDBITS];
} fd_set;

#define FD_SET(fd, set)   ((set)->fds_bits[(fd) / NFDBITS] |= (1U << ((fd) % NFDBITS)))
#define FD_CLR(fd, set)   ((set)->fds_bits[(fd) / NFDBITS] &= ~(1U << ((fd) % NFDBITS)))
#define FD_ISSET(fd, set) (((set)->fds_bits[(fd) / NFDBITS] & (1U << ((fd) % NFDBITS))) != 0)
#define FD_ZERO(set)      __builtin_memset((set), 0, sizeof(*(set)))

int select(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict,
           struct timeval *__restrict);
int pselect(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict,
            const struct timespec *__restrict, const sigset_t *__restrict);

#endif
