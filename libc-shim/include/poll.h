#ifndef _SLATE_POLL_H
#define _SLATE_POLL_H

#include <features.h>

#if defined(__SLATE_LIBC_DARWIN)

#include <bits/darwin/poll.h>

#elif defined(__SLATE_LIBC_FREEBSD)

#include <bits/freebsd/poll.h>

#else

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/poll.h>
#else
#include <bits/poll.h>
#endif
#endif

#if defined(__SLATE_LIBC_GLIBC)
#define POLLREMOVE 0x1000
#endif

#endif
