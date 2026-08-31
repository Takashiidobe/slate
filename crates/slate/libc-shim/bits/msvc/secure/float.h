#ifndef _SLATE_BITS_MSVC_SECURE_FLOAT_H
#define _SLATE_BITS_MSVC_SECURE_FLOAT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/secure/float.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

errno_t _controlfp_s(unsigned int *, unsigned int, unsigned int);

#endif
