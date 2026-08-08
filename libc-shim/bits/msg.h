#ifndef _SLATE_BITS_MSG_H
#define _SLATE_BITS_MSG_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msg.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/msg.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/msg.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/msg.h>
#else
#endif

#endif /* _SLATE_BITS_MSG_H */