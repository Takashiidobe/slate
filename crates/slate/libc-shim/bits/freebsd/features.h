#ifndef _SLATE_BITS_FREEBSD_FEATURES_H
#define _SLATE_BITS_FREEBSD_FEATURES_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/freebsd/features.h> directly; include a public header instead."
#endif

#if __SLATE_FREEBSD_VERSION__ != 1501000
#error "Slate only models FreeBSD 15.1-RELEASE."
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE == 1
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199009
#elif defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE == 2
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199209
#endif

#ifdef _XOPEN_SOURCE
#if _XOPEN_SOURCE - 0 >= 800
#define __XSI_VISIBLE 800
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 202405
#elif _XOPEN_SOURCE - 0 >= 700
#define __XSI_VISIBLE 700
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809
#elif _XOPEN_SOURCE - 0 >= 600
#define __XSI_VISIBLE 600
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112
#elif _XOPEN_SOURCE - 0 >= 500
#define __XSI_VISIBLE 500
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199506
#endif
#endif

#if defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 198808
#endif

#ifdef _POSIX_C_SOURCE
#if _POSIX_C_SOURCE >= 202405
#define __POSIX_VISIBLE 202405
#define __ISO_C_VISIBLE 2017
#elif _POSIX_C_SOURCE >= 200809
#define __POSIX_VISIBLE 200809
#define __ISO_C_VISIBLE 1999
#elif _POSIX_C_SOURCE >= 200112
#define __POSIX_VISIBLE 200112
#define __ISO_C_VISIBLE 1999
#elif _POSIX_C_SOURCE >= 199506
#define __POSIX_VISIBLE 199506
#define __ISO_C_VISIBLE 1990
#elif _POSIX_C_SOURCE >= 199309
#define __POSIX_VISIBLE 199309
#define __ISO_C_VISIBLE 1990
#elif _POSIX_C_SOURCE >= 199209
#define __POSIX_VISIBLE 199209
#define __ISO_C_VISIBLE 1990
#elif _POSIX_C_SOURCE >= 199009
#define __POSIX_VISIBLE 199009
#define __ISO_C_VISIBLE 1990
#else
#define __POSIX_VISIBLE 198808
#define __ISO_C_VISIBLE 0
#endif

#if _ISOC23_SOURCE || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L)
#undef __ISO_C_VISIBLE
#define __ISO_C_VISIBLE 2023
#elif _ISOC11_SOURCE || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
#undef __ISO_C_VISIBLE
#define __ISO_C_VISIBLE 2011
#endif
#else
#if defined(_ANSI_SOURCE)
#define __POSIX_VISIBLE 0
#define __XSI_VISIBLE 0
#define __BSD_VISIBLE 0
#define __ISO_C_VISIBLE 1990
#define __EXT1_VISIBLE 0
#elif defined(_C99_SOURCE)
#define __POSIX_VISIBLE 0
#define __XSI_VISIBLE 0
#define __BSD_VISIBLE 0
#define __ISO_C_VISIBLE 1999
#define __EXT1_VISIBLE 0
#elif defined(_C11_SOURCE)
#define __POSIX_VISIBLE 0
#define __XSI_VISIBLE 0
#define __BSD_VISIBLE 0
#define __ISO_C_VISIBLE 2011
#define __EXT1_VISIBLE 0
#elif defined(_C23_SOURCE)
#define __POSIX_VISIBLE 0
#define __XSI_VISIBLE 0
#define __BSD_VISIBLE 0
#define __ISO_C_VISIBLE 2023
#define __EXT1_VISIBLE 0
#else
#define __POSIX_VISIBLE 202405
#define __XSI_VISIBLE 800
#define __BSD_VISIBLE 1
#define __ISO_C_VISIBLE 2023
#define __EXT1_VISIBLE 1
#endif
#endif

#if defined(__STDC_WANT_LIB_EXT1__)
#undef __EXT1_VISIBLE
#if __STDC_WANT_LIB_EXT1__
#define __EXT1_VISIBLE 1
#else
#define __EXT1_VISIBLE 0
#endif
#endif

#ifndef __BSD_VISIBLE
#define __BSD_VISIBLE 0
#endif
#ifndef __XSI_VISIBLE
#define __XSI_VISIBLE 0
#endif
#ifndef __EXT1_VISIBLE
#define __EXT1_VISIBLE 0
#endif

#define __SLATE_FREEBSD_STRING_INNER(value) #value
#define __SLATE_FREEBSD_STRING(value) __SLATE_FREEBSD_STRING_INNER(value)
#define __sym_compat(symbol, implementation, version)                          \
  __asm__(".symver " #implementation ", " #symbol "@" #version)

#endif
