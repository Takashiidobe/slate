#ifndef _SLATE_BITS_SIGNAL_H
#define _SLATE_BITS_SIGNAL_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/signal.h> directly; include <signal.h> instead."
#endif

#if defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/signal.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/signal.h>
#elif defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/signal.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/signal.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/riscv64/signal.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/signal.h>
#else
#include <bits/generic/signal.h>
#endif

#endif
