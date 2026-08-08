#ifndef _SLATE_BITS_LIMITS_H
#define _SLATE_BITS_LIMITS_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/limits.h> directly; include <limits.h> instead."
#endif

#if defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/limits.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/limits.h>
#elif defined(__SLATE_ARCH_AARCH64) || defined(__SLATE_ARCH_ARM) ||            \
    defined(__SLATE_ARCH_RISCV64) || defined(__SLATE_ARCH_RISCV32)
#include <bits/generic/limits.h>
#else
#include <bits/generic/limits.h>
#endif

#endif
