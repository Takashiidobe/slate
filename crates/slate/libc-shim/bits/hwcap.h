#ifndef _SLATE_BITS_HWCAP_H
#define _SLATE_BITS_HWCAP_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/hwcap.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_AARCH64)
#include <bits/aarch64/hwcap.h>
#elif defined(__SLATE_ARCH_ARM)
#include <bits/arm/hwcap.h>
#else
#endif

#endif /* _SLATE_BITS_HWCAP_H */