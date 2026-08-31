#ifndef _SLATE_SYS_SYSCTL_H
#define _SLATE_SYS_SYSCTL_H

#include <features.h>

#if defined(__SLATE_LIBC_FREEBSD)

#define __NEED_size_t
#include <bits/types.h>

#include <bits/freebsd/sysctl.h>

#endif

#endif
