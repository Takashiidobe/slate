#ifndef _SLATE_SYS_POLL_H
#define _SLATE_SYS_POLL_H

#if defined(__SLATE_LIBC_MUSL)
#include <poll.h>
#endif
#include <bits/poll.h>

#if defined(__SLATE_LIBC_GLIBC)
#define POLLREMOVE 0x1000
#endif

#endif
