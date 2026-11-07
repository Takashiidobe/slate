#include <stdarg.h>
#include "dylink.h"
#include "long_double_abi/ld_abi.h"

EXPORT long double ld_identity(long double x) { return x; }

EXPORT long double ld_sum10(long double a0, long double a1, long double a2, long double a3,
                            long double a4, long double a5, long double a6, long double a7,
                            long double a8, long double a9) {
  return ((((a0 + a1) + a2) + a3) + a4) + ((((a5 + a6) + a7) + a8) + a9);
}

EXPORT long double ld_mix(int64_t i0, double d0, long double a, uint32_t u0, long double b,
                          float f0, long double c, int i1, double d1, long double d) {
  long double acc = a;
  acc = acc + b;
  acc = acc - c;
  acc = acc + d;
  acc = acc + (long double)i0;
  acc = acc - (long double)u0;
  acc = acc + (long double)d0;
  acc = acc - (long double)f0;
  acc = acc + (long double)i1;
  acc = acc - (long double)d1;
  return acc;
}

EXPORT struct ld_pair ld_pair_swap(struct ld_pair p) {
  struct ld_pair r = {p.b + 1.0L, p.a + 2.0L};
  return r;
}

EXPORT struct ld_one ld_one_half(struct ld_one o) {
  o.v = o.v / 2.0L + 0x1p-63L;
  return o;
}

EXPORT struct ld_box ld_box_bump(struct ld_box b) {
  b.tag ^= 0x5a;
  b.x = -b.x * 3.0L;
  b.tail += 7;
  return b;
}

EXPORT long double ld_call(ld_binary_fn fn, long double a, long double b) { return fn(a, b) * 2.0L; }

EXPORT long double ld_vsum(long double scale, int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = 0;
  for (int i = 0; i < count; i++) total += va_arg(ap, long double);
  va_end(ap);
  return total * scale;
}

EXPORT size_t ld_box_layout(size_t *x, size_t *tail) {
  *x = offsetof(struct ld_box, x);
  *tail = offsetof(struct ld_box, tail);
  return sizeof(struct ld_box);
}
