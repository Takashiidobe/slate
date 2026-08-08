#ifndef _SLATE_BITS_IOCTL_H
#define _SLATE_BITS_IOCTL_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/ioctl.h> directly; include <ioctl.h> instead."
#endif

#if defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/ioctl.h>
#elif defined(__SLATE_ARCH_X86) || defined(__SLATE_ARCH_AARCH64) ||            \
    defined(__SLATE_ARCH_ARM) || defined(__SLATE_ARCH_RISCV64) ||              \
    defined(__SLATE_ARCH_RISCV32)
#include <bits/generic/ioctl.h>
#else
#include <bits/generic/ioctl.h>
#endif

#endif
