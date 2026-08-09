#if !defined(__SLATE_LIBC_MSVC)
#include_next <stddef.h>
#else
#ifndef _SLATE_STDDEF_H
#define _SLATE_STDDEF_H

typedef __PTRDIFF_TYPE__ ptrdiff_t;
typedef __SIZE_TYPE__    size_t;
typedef __WCHAR_TYPE__   wchar_t;
typedef long double      max_align_t;

#include <bits/msvc/stddef.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define NULL nullptr
#else
#define NULL ((void *)0)
#endif

#define offsetof(type, member) __builtin_offsetof(type, member)

#endif
#endif
