#ifndef _SLATE_FEATURES_H
#define _SLATE_FEATURES_H

#define _SLATE_LIBC 1

#if defined(__SLATE_ARCH_X86_64) + defined(__SLATE_ARCH_X86) +                 \
        defined(__SLATE_ARCH_AARCH64) + defined(__SLATE_ARCH_ARM) +            \
        defined(__SLATE_ARCH_RISCV64) + defined(__SLATE_ARCH_RISCV32) !=       \
    1
#error "Slate requires one supported target architecture."
#endif

#if defined(__SLATE_VENDOR_UNKNOWN) + defined(__SLATE_VENDOR_PC) +             \
        defined(__SLATE_VENDOR_APPLE) !=                                       \
    1
#error "Slate requires one supported target vendor."
#endif

#if defined(__SLATE_KERNEL_LINUX) + defined(__SLATE_KERNEL_WINDOWS) +          \
        defined(__SLATE_KERNEL_DARWIN) !=                                      \
    1
#error "Slate requires one supported target kernel."
#endif

#if defined(__SLATE_LIBC_GLIBC) + defined(__SLATE_LIBC_MUSL) +                 \
        defined(__SLATE_LIBC_MINGW) + defined(__SLATE_LIBC_MSVC) +             \
        defined(__SLATE_LIBC_GENERIC) !=                                       \
    1
#error "Slate requires one supported target libc."
#endif

#if defined(__SLATE_OBJ_ELF) + defined(__SLATE_OBJ_COFF) +                     \
        defined(__SLATE_OBJ_MACHO) !=                                          \
    1
#error "Slate requires one supported target object format."
#endif

#if defined(__SLATE_WORDSIZE_64) + defined(__SLATE_WORDSIZE_32) != 1
#error "Slate requires one supported target word size."
#endif

#if defined(__SLATE_ENDIAN_LITTLE) + defined(__SLATE_ENDIAN_BIG) != 1
#error "Slate requires one target byte order."
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

#ifdef __SLATE_LIBC_GLIBC
#include <bits/glibc.h>
#endif

#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#elif !defined(__GNUC__)
#define __restrict
#endif

#if __STDC_VERSION__ >= 199901L
#define __inline inline
#elif !defined(__GNUC__)
#define __inline
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __noreturn [[noreturn]]
#elif __STDC_VERSION__ >= 201112L
#include <stdnoreturn.h>
#define __noreturn noreturn
#elif defined(__GNUC__)
#define __noreturn __attribute__((__noreturn__))
#else
#define __noreturn
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __aligned(X) [[aligned(X)]]
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define __aligned(X) _Alignas(X)
#elif defined(__GNUC__) || defined(__clang__)
#define __aligned(X) __attribute__((aligned(X)))
#elif defined(_MSC_VER)
#define __aligned(X) __declspec(align(X))
#else
#define __aligned(X)
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __align_of(T) alignof(T)
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define __align_of(T) _Alignof(T)
#elif defined(__GNUC__) || defined(__clang__)
#define __align_of(T) __alignof__(T)
#elif defined(_MSC_VER)
#define __align_of(T) __alignof(T)
#else
#define __align_of(T)                                                          \
  offsetof(                                                                    \
      struct {                                                                 \
        char c;                                                                \
        T    member;                                                           \
      },                                                                       \
      member)
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __deprecated(...) [[deprecated(__VA_ARGS__)]]
#elif defined(__GNUC__) || defined(__clang__)
#define __deprecated(...) __attribute__((deprecated(__VA_ARGS__)))
#elif defined(_MSC_VER)
#define __deprecated(...) __declspec(deprecated)
#else
#define __deprecated(...)
#endif

#ifndef __macro_warn
#if defined(__GNUC__) || defined(__clang__)
#define __macro_warn(msg) _Pragma(#msg)
#else
#define __macro_warn(msg)
#endif
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#if defined(__GNUC__) || defined(__clang__)
#define __packed [[gnu::packed]]
#else
#define __packed
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define __packed __attribute__((packed))
#else
#define __packed
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __const [[gnu::const]]
#elif defined(__GNUC__) || defined(__clang__)
#define __const __attribute__((const))
#elif defined(_MSC_VER)
#define __const __declspec(noalias)
#else
#define __const
#endif

#define __REDIR(x, y) __typeof__(x) x __asm__(#y)

#endif /* _SLATE_FEATURES_H */
