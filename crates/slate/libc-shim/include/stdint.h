#ifndef _SLATE_STDINT_H
#define _SLATE_STDINT_H

#include <features.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __STDC_VERSION_STDINT_H__ 202311L
#endif

#define __NEED_int8_t
#define __NEED_int16_t
#define __NEED_int32_t
#define __NEED_int64_t

#define __NEED_uint8_t
#define __NEED_uint16_t
#define __NEED_uint32_t
#define __NEED_uint64_t

#define __NEED_intptr_t
#define __NEED_uintptr_t

#define __NEED_intmax_t
#define __NEED_uintmax_t

#define __NEED_int_fast8_t
#define __NEED_int_fast16_t
#define __NEED_int_fast32_t
#define __NEED_int_fast64_t

#define __NEED_uint_fast8_t
#define __NEED_uint_fast16_t
#define __NEED_uint_fast32_t
#define __NEED_uint_fast64_t

#define __NEED_int_least8_t
#define __NEED_int_least16_t
#define __NEED_int_least32_t
#define __NEED_int_least64_t

#define __NEED_uint_least8_t
#define __NEED_uint_least16_t
#define __NEED_uint_least32_t
#define __NEED_uint_least64_t
#include <bits/types.h>

#define INT8_MIN  (-1 - 0x7f)
#define INT16_MIN (-1 - 0x7fff)
#define INT32_MIN (-1 - 0x7fffffff)
#define INT64_MIN (-1 - 0x7fffffffffffffff)

#define INT8_MAX  (0x7f)
#define INT16_MAX (0x7fff)
#define INT32_MAX (0x7fffffff)
#define INT64_MAX (0x7fffffffffffffff)

#define UINT8_MAX  (0xff)
#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffffu)
#if defined(__SLATE_LIBC_DARWIN)
#define UINT64_MAX (0xffffffffffffffffULL)
#else
#define UINT64_MAX (0xffffffffffffffffu)
#endif

#define INT_FAST8_MIN INT8_MIN
#if defined(__SLATE_LIBC_DARWIN)
#define INT_FAST16_MIN INT16_MIN
#define INT_FAST32_MIN INT32_MIN
#else
#define INT_FAST16_MIN INT64_MIN
#define INT_FAST32_MIN INT64_MIN
#endif
#define INT_FAST64_MIN INT64_MIN

#define INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX
#if defined(__SLATE_LIBC_DARWIN)
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST32_MAX INT32_MAX
#else
#define INT_FAST16_MAX INT64_MAX
#define INT_FAST32_MAX INT64_MAX
#endif
#define INT_FAST64_MAX INT64_MAX

#define INT_LEAST8_MAX  INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_FAST8_MAX UINT8_MAX
#if defined(__SLATE_LIBC_DARWIN)
#define UINT_FAST16_MAX UINT16_MAX
#define UINT_FAST32_MAX UINT32_MAX
#else
#define UINT_FAST16_MAX UINT64_MAX
#define UINT_FAST32_MAX UINT64_MAX
#endif
#define UINT_FAST64_MAX UINT64_MAX

#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INTMAX_MIN  INT64_MIN
#define INTMAX_MAX  INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#if defined(__SLATE_LIBC_MSVC) || defined(__LP64__)
#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX
#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX
#define SIZE_MAX    UINT64_MAX
#else
#define INTPTR_MIN  INT32_MIN
#define INTPTR_MAX  INT32_MAX
#define UINTPTR_MAX UINT32_MAX
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX
#define SIZE_MAX    UINT32_MAX
#endif

#if defined(__SLATE_LIBC_DARWIN)
#define WINT_MIN INT32_MIN
#define WINT_MAX INT32_MAX
#elif defined(__SLATE_LIBC_MSVC)
#define WINT_MIN 0U
#define WINT_MAX UINT16_MAX
#else
#define WINT_MIN 0U
#define WINT_MAX UINT32_MAX
#endif

#if defined(__SLATE_LIBC_MSVC)
#define WCHAR_MAX UINT16_MAX
#define WCHAR_MIN 0
#elif L'\0' - 1 > 0
#define WCHAR_MAX (0xffffffffu + L'\0')
#define WCHAR_MIN (0 + L'\0')
#else
#define WCHAR_MAX (0x7fffffff + L'\0')
#define WCHAR_MIN (-1 - 0x7fffffff + L'\0')
#endif

#define SIG_ATOMIC_MIN INT32_MIN
#define SIG_ATOMIC_MAX INT32_MAX

#define INT8_C(c)  c
#define INT16_C(c) c
#define INT32_C(c) c

#define UINT8_C(c)  c
#define UINT16_C(c) c
#define UINT32_C(c) c##U

#if defined(__SLATE_LIBC_DARWIN)
#define INT64_C(c)   c##LL
#define UINT64_C(c)  c##ULL
#define INTMAX_C(c)  c##L
#define UINTMAX_C(c) c##UL
#elif UINTPTR_MAX == UINT64_MAX
#define INT64_C(c)   c##L
#define UINT64_C(c)  c##UL
#define INTMAX_C(c)  c##L
#define UINTMAX_C(c) c##UL
#else
#define INT64_C(c)   c##LL
#define UINT64_C(c)  c##ULL
#define INTMAX_C(c)  c##LL
#define UINTMAX_C(c) c##ULL
#endif

#endif
