#ifndef _SLATE_UCHAR_H
#define _SLATE_UCHAR_H

#define __need_size_t
#include <stddef.h>
#undef __need_size_t

#include <stdint.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

typedef struct {
  int __count;
  union {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} mbstate_t;

size_t mbrtoc16(char16_t *restrict, const char *, size_t n,
                const mbstate_t *ps);
size_t c16rtomb(char *, char16_t, const mbstate_t *);
size_t mbrtoc32(char32_t *, const char *, size_t n, const mbstate_t *);
size_t c32rtomb(char *, char32_t, mbstate_t *);

#endif
