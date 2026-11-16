/* { dg-options "-O2 -ffreestanding" } */
#include "kstring.h"

__attribute__((noinline)) static size_t count_bytes(const char *s) {
  const char *sc;
  for (sc = s; *sc != '\0'; ++sc)
    ;
  return sc - s;
}

__attribute__((noinline)) static void fill_bytes(char *xs, int c, size_t count) {
  while (count--)
    *xs++ = c;
}

size_t strlen(const char *s) { return count_bytes(s); }

void *memset(void *s, int c, size_t count) {
  fill_bytes(s, c, count);
  return s;
}

size_t padded_length(char *buf, size_t width) {
  memset(buf, '.', width);
  buf[width] = '\0';
  return strlen(buf);
}
