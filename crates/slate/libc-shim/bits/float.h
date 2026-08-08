#ifndef _SLATE_BITS_FLOAT_H
#define _SLATE_BITS_FLOAT_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/float.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/float.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/float.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/float.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/riscv64/float.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/float.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/float.h>
#else
#endif

#endif /* _SLATE_BITS_FLOAT_H */