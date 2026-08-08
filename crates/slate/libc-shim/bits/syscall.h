#ifndef _SLATE_BITS_SYSCALL_H
#define _SLATE_BITS_SYSCALL_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/syscall.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/syscall.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/syscall.h>
#else
#endif

#endif /* _SLATE_BITS_SYSCALL_H */