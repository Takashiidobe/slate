#ifndef _SLATE_FEATURES_H
#define _SLATE_FEATURES_H

#define _SLATE_LIBC 1

#if defined(__x86_64__)
#define __SLATE_ARCH_X86_64 1
#define __SLATE_ARCH "x86_64"
#elif defined(__i386__)
#define __SLATE_ARCH_X86 1
#define __SLATE_ARCH "x86"
#elif defined(__aarch64__)
#define __SLATE_ARCH_AARCH64 1
#define __SLATE_ARCH "aarch64"
#elif defined(__arm__)
#define __SLATE_ARCH_ARM 1
#define __SLATE_ARCH "arm"
#elif defined(__riscv) && __riscv_xlen == 64
#define __SLATE_ARCH_RISCV64 1
#define __SLATE_ARCH "riscv64"
#elif defined(__riscv) && __riscv_xlen == 32
#define __SLATE_ARCH_RISCV32 1
#define __SLATE_ARCH "riscv32"
#else
#error "Slate only supports x86_64, x86_32, aarch64, arm32, riscv64, riscv32."
#endif

#endif /* _SLATE_FEATURES_H */
