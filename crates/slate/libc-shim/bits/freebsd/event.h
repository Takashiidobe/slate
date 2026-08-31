#ifndef _SLATE_BITS_FREEBSD_EVENT_H
#define _SLATE_BITS_FREEBSD_EVENT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/event.h> directly; include a public header instead."
#endif

#define EVFILT_READ     (-1)
#define EVFILT_WRITE    (-2)
#define EVFILT_AIO      (-3)
#define EVFILT_VNODE    (-4)
#define EVFILT_PROC     (-5)
#define EVFILT_SIGNAL   (-6)
#define EVFILT_TIMER    (-7)
#define EVFILT_PROCDESC (-8)
#define EVFILT_FS       (-9)
#define EVFILT_LIO      (-10)
#define EVFILT_USER     (-11)
#define EVFILT_SENDFILE (-12)
#define EVFILT_EMPTY    (-13)
#define EVFILT_SYSCOUNT 13

struct kevent {
  uintptr_t      ident;
  short          filter;
  unsigned short flags;
  unsigned int   fflags;
  int64_t        data;
  void          *udata;
  uint64_t       ext[4];
};

#define EV_SET(kevp_, a, b, c, d, e, f)                                       \
  do {                                                                        \
    struct kevent *kevp = (kevp_);                                            \
    (kevp)->ident = (a);                                                      \
    (kevp)->filter = (b);                                                     \
    (kevp)->flags = (c);                                                      \
    (kevp)->fflags = (d);                                                     \
    (kevp)->data = (e);                                                       \
    (kevp)->udata = (f);                                                      \
    (kevp)->ext[0] = 0;                                                       \
    (kevp)->ext[1] = 0;                                                       \
    (kevp)->ext[2] = 0;                                                       \
    (kevp)->ext[3] = 0;                                                       \
  } while (0)

#define EV_ADD         0x0001
#define EV_DELETE      0x0002
#define EV_ENABLE      0x0004
#define EV_DISABLE     0x0008
#define EV_FORCEONESHOT 0x0100
#define EV_KEEPUDATA   0x0200
#define EV_ONESHOT     0x0010
#define EV_CLEAR       0x0020
#define EV_RECEIPT     0x0040
#define EV_DISPATCH    0x0080
#define EV_SYSFLAGS    0xF000
#define EV_DROP        0x1000
#define EV_FLAG1       0x2000
#define EV_FLAG2       0x4000
#define EV_EOF         0x8000
#define EV_ERROR       0x4000

int kqueue(void);
int kevent(int, const struct kevent *, int, struct kevent *, int,
           const struct timespec *);

#endif
