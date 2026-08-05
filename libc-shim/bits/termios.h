#ifndef _SLATE_BITS_TERMIOS_H
#define _SLATE_BITS_TERMIOS_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/termios.h> directly; include <termios.h> instead."
#endif

#if defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/termios.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/termios.h>
#elif defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/termios.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/termios.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/riscv64/termios.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/termios.h>
#else
#endif

#endif /* _SLATE_BITS_TERMIOS_H */
