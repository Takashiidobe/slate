#ifndef _SLATE_SYS_SINGLE_THREADED_H
#define _SLATE_SYS_SINGLE_THREADED_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
extern char __libc_single_threaded;
#endif

#endif
