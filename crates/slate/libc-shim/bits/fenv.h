#ifndef _SLATE_BITS_FENV_H
#define _SLATE_BITS_FENV_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/fenv.h> directly; include <fenv.h> instead."
#endif

#if defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/fenv.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/fenv.h>
#elif defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/fenv.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/fenv.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/riscv64/fenv.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/fenv.h>
#else
#include <bits/generic/fenv.h>
#endif

#endif
