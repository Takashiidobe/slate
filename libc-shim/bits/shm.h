#ifndef _SLATE_BITS_SHM_H
#define _SLATE_BITS_SHM_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/shm.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/shm.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/shm.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/shm.h>
#else
#endif

#endif /* _SLATE_BITS_SHM_H */