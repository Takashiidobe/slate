#ifndef _SLATE_UCHAR_H
#define _SLATE_UCHAR_H

#define __need_size_t
#include <stddef.h>
#undef __need_size_t

#include <stdint.h>

#define __NEED_mbstate_t
#include <bits/types.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
typedef unsigned char char8_t;
#endif

size_t mbrtoc16(char16_t *restrict, const char *, size_t n,
                const mbstate_t *ps);
size_t c16rtomb(char *, char16_t, const mbstate_t *);
size_t mbrtoc32(char32_t *, const char *, size_t n, const mbstate_t *);
size_t c32rtomb(char *, char32_t, mbstate_t *);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
size_t mbrtoc8(char8_t *restrict pc8, const char *restrict s, size_t n,
               mbstate_t *restrict ps);
size_t c8rtomb(char *restrict s, char8_t c8, mbstate_t *restrict ps);
#endif

#endif
