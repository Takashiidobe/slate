#ifndef _SLATE_BITS_USER_H
#define _SLATE_BITS_USER_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/user.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/user.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/user.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/user.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/riscv64/user.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/user.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/user.h>
#else
#endif

#endif /* _SLATE_BITS_USER_H */