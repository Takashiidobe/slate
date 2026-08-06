#ifndef _SLATE_FEATURES_TIME64_H
#define _SLATE_FEATURES_TIME64_H

#include <features.h>

#if defined _TIME_BITS
#if _TIME_BITS == 64
#if !defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS != 64
#error "_TIME_BITS=64 is allowed only with _FILE_OFFSET_BITS=64"
#endif
#define __USE_TIME_BITS64 1
#elif _TIME_BITS == 32
#if __TIMESIZE > 32
#error "_TIME_BITS=32 is not compatible with __TIMESIZE > 32"
#endif
#else
#error Invalid _TIME_BITS value (can only be 32 or 64-bit)
#endif
#elif __TIMESIZE == 64
#define __USE_TIME_BITS64 1
#endif

#if defined __USE_TIME_BITS64 && __TIMESIZE == 32
#define __USE_TIME64_REDIRECTS 1
#endif

#endif
