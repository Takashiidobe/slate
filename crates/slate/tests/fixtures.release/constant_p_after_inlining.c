// { dg-options "-O2" }
#include <stdio.h>

void __compiletime_assert_1(void);

#define is_nonneg(x) (__builtin_constant_p(x) && (x) >= 0)

static inline __attribute__((always_inline)) unsigned min_segments(unsigned a, int b) {
  int y = b;
  if (!is_nonneg(y))
    __compiletime_assert_1();
  return a < (unsigned)y ? a : (unsigned)y;
}

unsigned limit(unsigned segments) { return min_segments(segments, 2); }

int query(int v, double d) { return __builtin_constant_p(v) + __builtin_constant_p(d); }

int main(void) {
  printf("%u %u %d\n", limit(1), limit(9), query(3, 1.5));
  return 0;
}
