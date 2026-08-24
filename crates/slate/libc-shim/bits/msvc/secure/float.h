#ifndef _SLATE_BITS_MSVC_SECURE_FLOAT_H
#define _SLATE_BITS_MSVC_SECURE_FLOAT_H

#include <bits/msvc/types.h>

errno_t _controlfp_s(unsigned int *, unsigned int, unsigned int);

#endif
