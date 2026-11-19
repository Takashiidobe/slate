#include "dylink.h"
#include "../abi_mabi_ms.h"

EXPORT int MS ms_sub(int a, int b) { return a - b; }

EXPORT long long MS ms_wide(long long a, int b, short c) { return a * b + c; }

EXPORT int MS ms_apply(binary_fn f, int a, int b) {
  int total = 0;
  for (int i = 0; i < 1000; i++)
    total += f(a + i, b);
  return total;
}

EXPORT int MS ms_sum(int n, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, n);
  int total = 0;
  for (int i = 0; i < n; i++)
    total += __builtin_va_arg(ap, int);
  __builtin_ms_va_end(ap);
  return total;
}

EXPORT struct pair MS ms_make(int a, int b) {
  struct pair p = {a * 3, b - 1};
  return p;
}

EXPORT long long MS ms_spread(struct triple t, int a, int b, int c, int d) {
  return t.a * 1000000 + t.b * 1000 + t.c + a * 2 + b * 3 + c * 5 + d * 7;
}

EXPORT double MS ms_mix(float f, double d, int i) { return f * d + i; }
