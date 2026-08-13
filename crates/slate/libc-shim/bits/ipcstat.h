#ifndef _SLATE_BITS_IPCSTAT_H
#define _SLATE_BITS_IPCSTAT_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/ipcstat.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/ipcstat.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/ipcstat.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/ipcstat.h>
#else
#define IPC_STAT 2
#endif

#endif /* _SLATE_BITS_IPCSTAT_H */
