#ifndef _SLATE_BITS_DARWIN_FEATURES_H
#define _SLATE_BITS_DARWIN_FEATURES_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/darwin/features.h> directly; include a public header instead."
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE == 1L
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199009L
#elif defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE == 2L
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199209L
#endif

#if defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700L &&                        \
    (!defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L)
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#elif defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600L &&                      \
    (!defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200112L)
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#elif defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500L &&                      \
    (!defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 199506L)
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199506L
#endif

#if defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 198808L
#endif

#define __DARWIN_C_ANSI 010000L
#define __DARWIN_C_FULL 900000L

#if defined(_ANSI_SOURCE)
#define __DARWIN_C_LEVEL __DARWIN_C_ANSI
#elif defined(_POSIX_C_SOURCE) && !defined(_DARWIN_C_SOURCE)
#define __DARWIN_C_LEVEL _POSIX_C_SOURCE
#else
#define __DARWIN_C_LEVEL __DARWIN_C_FULL
#endif

#define __DARWIN_UNIX03 1
#define __DARWIN_64_BIT_INO_T 1
#define __DARWIN_VERS_1050 1
#define __DARWIN_NON_CANCELABLE 0
#define __DARWIN_SUF_UNIX03
#define __DARWIN_SUF_64_BIT_INO_T
#define __DARWIN_SUF_1050
#define __DARWIN_SUF_NON_CANCELABLE
#define __DARWIN_SUF_EXTSN "$DARWIN_EXTSN"
#define __SLATE_STRING_INNER(value) #value
#define __SLATE_STRING(value) __SLATE_STRING_INNER(value)
#define __DARWIN_ALIAS(symbol)                                                 \
  __asm("_" __SLATE_STRING(symbol) __DARWIN_SUF_UNIX03)
#define __DARWIN_ALIAS_C(symbol)                                               \
  __asm("_" __SLATE_STRING(symbol) __DARWIN_SUF_NON_CANCELABLE               \
            __DARWIN_SUF_UNIX03)
#define __DARWIN_ALIAS_I(symbol)                                               \
  __asm("_" __SLATE_STRING(symbol) __DARWIN_SUF_64_BIT_INO_T                 \
            __DARWIN_SUF_UNIX03)
#define __DARWIN_NOCANCEL(symbol)                                              \
  __asm("_" __SLATE_STRING(symbol) __DARWIN_SUF_NON_CANCELABLE)
#define __DARWIN_INODE64(symbol)                                               \
  __asm("_" __SLATE_STRING(symbol) __DARWIN_SUF_64_BIT_INO_T)
#define __DARWIN_EXTSN(symbol)                                                 \
  __asm("_" __SLATE_STRING(symbol) __DARWIN_SUF_EXTSN)

#endif
