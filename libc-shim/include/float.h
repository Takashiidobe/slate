#ifndef _SLATE_FLOAT_H
#define _SLATE_FLOAT_H

#include_next <float.h>

#include <features.h>

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/float.h>
#endif

#endif
