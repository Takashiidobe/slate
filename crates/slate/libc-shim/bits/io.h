#ifndef _SLATE_BITS_IO_H
#define _SLATE_BITS_IO_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/io.h> directly; include a standard header instead."
#endif

#if defined(__SLATE_ARCH_X86)
#include <bits/x86/io.h>
#elif defined(__SLATE_ARCH_X86_64)
#include <bits/x86_64/io.h>
#else
#endif

#endif /* _SLATE_BITS_IO_H */