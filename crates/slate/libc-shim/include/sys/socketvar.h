#ifndef _SLATE_SYS_SOCKETVAR_H
#define _SLATE_SYS_SOCKETVAR_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/socket.h>
#else
#error "<sys/socketvar.h> is unavailable for this libc profile."
#endif

#endif
