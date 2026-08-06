#ifndef _SLATE_GLIBC_H
#define _SLATE_GLIBC_H

#define __GLIBC_USE(F) __GLIBC_USE_##F

#ifdef _GNU_SOURCE
#define __USE_GNU 1
#endif

#if defined(_DEFAULT_SOURCE)
#define __USE_MISC 1
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)
#define __USE_POSIX 1
#endif

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 2) || defined(_XOPEN_SOURCE)
#define __USE_POSIX2 1
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L
#define __USE_POSIX199309 1
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199506L
#define __USE_POSIX199506 1
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L
#define __USE_XOPEN2K 1
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L
#define __USE_XOPEN2K8 1
#endif

#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 202405L
#define __USE_XOPEN2K24 1
#endif

#ifdef _XOPEN_SOURCE
#define __USE_XOPEN 1
#if (_XOPEN_SOURCE - 0) >= 500
#define __USE_XOPEN_EXTENDED 1
#define __USE_UNIX98         1
#if (_XOPEN_SOURCE - 0) >= 600
#define __USE_XOPEN2K    1
#define __USE_XOPEN2KXSI 1
#if (_XOPEN_SOURCE - 0) >= 700
#define __USE_XOPEN2K8    1
#define __USE_XOPEN2K8XSI 1
#if (_XOPEN_SOURCE - 0) >= 800
#define __USE_XOPEN2K24    1
#define __USE_XOPEN2K24XSI 1
#endif
#endif
#endif
#endif
#endif

#ifdef _LARGEFILE_SOURCE
#define __USE_LARGEFILE 1
#endif

#ifdef _LARGEFILE64_SOURCE
#define __USE_LARGEFILE64 1
#endif

#if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64
#define __USE_FILE_OFFSET64 1
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define __USE_ISOC99 1
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define __USE_ISOC11 1
#endif

#if (defined(_ISOC23_SOURCE) || defined(_ISOC2Y_SOURCE) ||                     \
     (defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L))
#define __GLIBC_USE_ISOC23 1
#else
#define __GLIBC_USE_ISOC23 0
#endif

#if (defined(_ISOC2Y_SOURCE) ||                                                \
     (defined(__STDC_VERSION__) && __STDC_VERSION__ > 202311L))
#define __GLIBC_USE_ISOC2Y 1
#else
#define __GLIBC_USE_ISOC2Y 0
#endif

#if defined(__USE_GNU) || defined(__STDC_WANT_IEC_60559_BFP_EXT__)
#define __GLIBC_USE_IEC_60559_BFP_EXT 1
#else
#define __GLIBC_USE_IEC_60559_BFP_EXT 0
#endif

#if __GLIBC_USE_IEC_60559_BFP_EXT || __GLIBC_USE_ISOC23
#define __GLIBC_USE_IEC_60559_BFP_EXT_C23 1
#else
#define __GLIBC_USE_IEC_60559_BFP_EXT_C23 0
#endif

#if __GLIBC_USE_IEC_60559_BFP_EXT || defined(__STDC_WANT_IEC_60559_EXT__)
#define __GLIBC_USE_IEC_60559_EXT 1
#else
#define __GLIBC_USE_IEC_60559_EXT 0
#endif

#if defined(__USE_GNU) || defined(__STDC_WANT_IEC_60559_FUNCS_EXT__)
#define __GLIBC_USE_IEC_60559_FUNCS_EXT 1
#else
#define __GLIBC_USE_IEC_60559_FUNCS_EXT 0
#endif

#if __GLIBC_USE_IEC_60559_FUNCS_EXT || __GLIBC_USE_ISOC23
#define __GLIBC_USE_IEC_60559_FUNCS_EXT_C23 1
#else
#define __GLIBC_USE_IEC_60559_FUNCS_EXT_C23 0
#endif

#if defined(__USE_GNU) ||                                                      \
    (defined(__STDC_WANT_LIB_EXT2__) && __STDC_WANT_LIB_EXT2__ > 0)
#define __GLIBC_USE_LIB_EXT2 1
#else
#define __GLIBC_USE_LIB_EXT2 0
#endif

#if defined(__cplusplus) ? __cplusplus >= 201402L : defined(__USE_ISOC11)
#define __GLIBC_USE_DEPRECATED_GETS 0
#else
#define __GLIBC_USE_DEPRECATED_GETS 1
#endif

#if defined(__USE_GNU) &&                                                      \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#define __GLIBC_USE_DEPRECATED_SCANF 1
#else
#define __GLIBC_USE_DEPRECATED_SCANF 0
#endif

#if __GLIBC_USE_ISOC23
#define __GLIBC_USE_C23_STRTOL 1
#else
#define __GLIBC_USE_C23_STRTOL 0
#endif

#define __USE_FORTIFY_LEVEL 0

#endif
