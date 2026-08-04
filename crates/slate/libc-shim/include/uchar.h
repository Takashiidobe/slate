#ifndef _SLATE_UCHAR_H
#define _SLATE_UCHAR_H

#define __need_size_t
#include <stddef.h>
#undef __need_size_t

typedef __CHAR16_TYPE__ char16_t;
typedef __CHAR32_TYPE__ char32_t;

/* Opaque conversion state, sized/aligned to match the real glibc
   __mbstate_t (an int tag plus a 4-byte value union) since the real
   mbrtoc16/mbrtoc32/c16rtomb/c32rtomb symbols read and write it. */
typedef struct {
  int __count;
  union {
    unsigned int __wch;
    char __wchb[4];
  } __value;
} mbstate_t;

size_t mbrtoc16(char16_t *restrict pc16, const char *restrict s, size_t n,
                mbstate_t *restrict ps);
size_t c16rtomb(char *restrict s, char16_t c16, mbstate_t *restrict ps);
size_t mbrtoc32(char32_t *restrict pc32, const char *restrict s, size_t n,
                mbstate_t *restrict ps);
size_t c32rtomb(char *restrict s, char32_t c32, mbstate_t *restrict ps);

#endif
