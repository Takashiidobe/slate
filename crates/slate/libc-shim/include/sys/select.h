#ifndef _SLATE_SYS_SELECT_H
#define _SLATE_SYS_SELECT_H

#include <features.h>

#define __NEED_size_t
#define __NEED_time_t
#define __NEED_suseconds_t
#define __NEED_struct_timeval
#define __NEED_struct_timespec
#define __NEED_sigset_t
#include <bits/types.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/select.h>

#else

enum { FD_SETSIZE = 1024 };

typedef unsigned long fd_mask;

typedef struct {
  unsigned long fds_bits[FD_SETSIZE / 8 / sizeof(long)];
} fd_set;

void FD_ZERO(fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_CLR(int fd, fd_set *set);
int  FD_ISSET(int fd, fd_set *set);

int select(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict,
           struct timeval *__restrict);
int pselect(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict,
            const struct timespec *__restrict, const sigset_t *__restrict);

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
enum { NFDBITS = (8 * (int)sizeof(long)) };
#endif

#if _REDIR_TIME64
__REDIR(select, __select_time64);
__REDIR(pselect, __pselect_time64);
#endif

#endif

#endif
