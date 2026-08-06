#ifndef _SLATE_FEATURES_H
#define _SLATE_FEATURES_H

#define _SLATE_LIBC 1

#if defined(__x86_64__)
#define __SLATE_ARCH_X86_64 1
#define __SLATE_ARCH        "x86_64"
#define __TIMESIZE          64
#define __WORDSIZE          64
#elif defined(__i386__)
#define __SLATE_ARCH_X86 1
#define __SLATE_ARCH     "x86"
#define __TIMESIZE       32
#define __WORDSIZE       32
#elif defined(__aarch64__)
#define __SLATE_ARCH_AARCH64 1
#define __SLATE_ARCH         "aarch64"
#define __TIMESIZE           64
#define __WORDSIZE           64
#elif defined(__arm__)
#define __SLATE_ARCH_ARM 1
#define __SLATE_ARCH     "arm"
#define __TIMESIZE       32
#define __WORDSIZE       32
#elif defined(__riscv) && __riscv_xlen == 64
#define __SLATE_ARCH_RISCV64 1
#define __SLATE_ARCH         "riscv64"
#define __TIMESIZE           64
#define __WORDSIZE           64
#elif defined(__riscv) && __riscv_xlen == 32
#define __SLATE_ARCH_RISCV32 1
#define __SLATE_ARCH         "riscv32"
#define __TIMESIZE           32
#define __WORDSIZE           32
#else
#error "Slate only supports x86_64, x86_32, aarch64, arm32, riscv64, riscv32."
#endif

#if !defined(__SLATE_LIBC_MUSL) && !defined(__SLATE_LIBC_GNU)
#define __SLATE_LIBC_GENERIC
#endif

#if defined(_ALL_SOURCE) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif

#if defined(_DEFAULT_SOURCE) && !defined(_BSD_SOURCE)
#define _BSD_SOURCE 1
#endif

#if !defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE) &&                    \
    !defined(_XOPEN_SOURCE) && !defined(_GNU_SOURCE) &&                        \
    !defined(_BSD_SOURCE) && !defined(__STRICT_ANSI__)
#define _BSD_SOURCE   1
#define _XOPEN_SOURCE 700
#endif

#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#elif !defined(__GNUC__)
#define __restrict
#endif

#if __STDC_VERSION__ >= 199901L || defined(__cplusplus)
#define __inline inline
#elif !defined(__GNUC__)
#define __inline
#endif

#if __STDC_VERSION__ >= 201112L
#elif defined(__GNUC__)
#define _Noreturn __attribute__((__noreturn__))
#else
#define _Noreturn
#endif

#define __REDIR(x, y) __typeof__(x) x __asm__(#y)

#endif /* _SLATE_FEATURES_H */
