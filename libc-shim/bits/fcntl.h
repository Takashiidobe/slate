#ifndef _SLATE_BITS_FCNTL_H
#define _SLATE_BITS_FCNTL_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/fcntl.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/fcntl.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/fcntl.h>
#else
#include <bits/generic/fcntl.h>
#endif

#endif /* _SLATE_BITS_FCNTL_H */