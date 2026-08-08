#ifndef _SLATE_BITS_SETJMP_H
#define _SLATE_BITS_SETJMP_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/setjmp.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/setjmp.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/setjmp.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/setjmp.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/riscv64/setjmp.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/setjmp.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/setjmp.h>
#else
#endif

#endif /* _SLATE_BITS_SETJMP_H */