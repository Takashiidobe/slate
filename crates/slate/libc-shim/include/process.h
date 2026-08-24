#ifndef _SLATE_PROCESS_H
#define _SLATE_PROCESS_H

#include <features.h>

#if defined(__SLATE_LIBC_MSVC)

#define __NEED_intptr_t
#define __NEED_uintptr_t
#define __NEED_wchar_t
#include <bits/types.h>

#include <bits/msvc/process.h>

#endif

#endif
