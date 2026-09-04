#ifndef _SLATE_BITS_STAT_H
#define _SLATE_BITS_STAT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/stat.h> directly; include <stat.h> instead."
#endif

#if defined(__SLATE_LIBC_BIONIC)
#include <bits/bionic/stat.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/stat.h>
#elif defined(__SLATE_ARCH_X86)
#include <bits/x86/stat.h>
#elif defined(__SLATE_ARCH_AARCH64)
#include <bits/generic/stat.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/stat.h>
#elif defined(__SLATE_ARCH_RISCV64)
#include <bits/generic/stat.h>
#elif defined(__SLATE_ARCH_RISCV32)
#include <bits/riscv32/stat.h>
#else
#include <bits/generic/stat.h>
#endif

#endif
