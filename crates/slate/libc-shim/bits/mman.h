#ifndef _SLATE_BITS_MMAN_H
#define _SLATE_BITS_MMAN_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/mman.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/mman.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/mman.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/mman.h>
#else
#endif

#endif /* _SLATE_BITS_MMAN_H */