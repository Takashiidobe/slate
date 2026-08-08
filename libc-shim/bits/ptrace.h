#ifndef _SLATE_BITS_PTRACE_H
#define _SLATE_BITS_PTRACE_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/ptrace.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/ptrace.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/ptrace.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/ptrace.h>
#else
#endif

#endif /* _SLATE_BITS_PTRACE_H */