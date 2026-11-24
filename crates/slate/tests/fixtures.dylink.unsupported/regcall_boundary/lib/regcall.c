#include "dylink.h"
#include "../abi.h"

EXPORT long __attribute__((regcall))
weighted_sum(long, long, long, long, long, long) __asm__("weighted_sum");

EXPORT long __attribute__((regcall))
weighted_sum(long a, long b, long c, long d, long e, long f) {
  return a + 2 * b + 4 * c + 8 * d + 16 * e + 32 * f;
}

EXPORT int check_sum(sum_fn fn) { return fn(1, 2, 3, 4, 5, 6) == 321; }
