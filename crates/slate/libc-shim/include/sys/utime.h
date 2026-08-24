#ifndef _SLATE_SYS_UTIME_H
#define _SLATE_SYS_UTIME_H

#include <features.h>

#if defined(__SLATE_LIBC_MSVC)

#define __NEED_time_t
#define __NEED_wchar_t
#include <bits/types.h>

#include <bits/msvc/sys/utime.h>

#else

#include <utime.h>

#endif

#endif
