#ifndef _SLATE_SYS_EVENT_H
#define _SLATE_SYS_EVENT_H

#include <features.h>

#if defined(__SLATE_LIBC_FREEBSD)

#define __NEED_uintptr_t
#define __NEED_int64_t
#define __NEED_uint64_t
#define __NEED_struct_timespec
#include <bits/types.h>

#include <bits/freebsd/event.h>

#endif 

#endif
