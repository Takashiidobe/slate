#ifndef _SLATE_SYS_SENDFILE_H
#define _SLATE_SYS_SENDFILE_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/types.h>
#else
#include <unistd.h>
#endif

ssize_t sendfile(int, int, off_t *, size_t);

#if defined(_LARGEFILE64_SOURCE)
#define sendfile64 sendfile
#define off64_t    off_t
#endif

#endif
