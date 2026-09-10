#ifndef _SLATE_SYS_EPOLL_H
#define _SLATE_SYS_EPOLL_H

#if defined(__SLATE_LIBC_MUSL)
#include <fcntl.h>
#endif
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define __NEED_sigset_t
#include <bits/types.h>

#define EPOLL_CLOEXEC  02000000
#define EPOLL_NONBLOCK 00004000

#define EPOLLIN        0x001
#define EPOLLPRI       0x002
#define EPOLLOUT       0x004
#define EPOLLRDNORM    0x040
#define EPOLLNVAL      0x020
#define EPOLLRDBAND    0x080
#define EPOLLWRNORM    0x100
#define EPOLLWRBAND    0x200
#define EPOLLMSG       0x400
#define EPOLLERR       0x008
#define EPOLLHUP       0x010
#define EPOLLRDHUP     0x2000
#define EPOLLEXCLUSIVE (1U << 28)
#define EPOLLWAKEUP    (1U << 29)
#define EPOLLONESHOT   (1U << 30)
#define EPOLLET        (1U << 31)

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

typedef union epoll_data {
  void    *ptr;
  int      fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t     events;
  epoll_data_t data;
}
#if defined(__SLATE_ARCH_X86_64) || defined(__SLATE_ARCH_X86)
__packed
#endif
    ;

struct epoll_params {
  uint32_t busy_poll_usecs;
  uint16_t busy_poll_budget;
  uint8_t  prefer_busy_poll;

  uint8_t __pad;
};

#define EPOLL_IOC_TYPE 0x8A
#define EPIOCSPARAMS   _IOW(EPOLL_IOC_TYPE, 0x01, struct epoll_params)
#define EPIOCGPARAMS   _IOR(EPOLL_IOC_TYPE, 0x02, struct epoll_params)

int epoll_create(int);
int epoll_create1(int);
int epoll_ctl(int, int, int, struct epoll_event *);
int epoll_wait(int, struct epoll_event *, int, int);
int epoll_pwait(int, struct epoll_event *, int, int, const sigset_t *);

#endif /* sys/epoll.h */
