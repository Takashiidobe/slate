#include <stddef.h>
#include <stdio.h>

static void *first_word(void *p) {
  void *q = __builtin_assume_aligned(p, 32);
  return q;
}

static void *first_word_offset(void *p, size_t off) {
  void *q = __builtin_assume_aligned(p, 32, off);
  return q;
}

int main(void) {
  _Alignas(32) static unsigned char buf[64];
  void                             *a = first_word(buf);
  void                             *b = first_word_offset(buf + 8, 8);
  printf("%d %d\n", a == buf, b == buf + 8);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
