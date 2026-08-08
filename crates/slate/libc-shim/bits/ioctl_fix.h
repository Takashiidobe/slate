#ifndef _SLATE_BITS_IOCTL_FIX_H
#define _SLATE_BITS_IOCTL_FIX_H

#if !defined(_SLATE_LIBC)
#error                                                                         \
    "Never include <bits/ioctl_fix.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_ARM)
#include <bits/arm/ioctl_fix.h>
#else
#endif

#endif /* _SLATE_BITS_IOCTL_FIX_H */