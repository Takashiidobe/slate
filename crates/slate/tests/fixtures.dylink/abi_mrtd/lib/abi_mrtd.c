#include <stdarg.h>
#include "dylink.h"
#include "../abi_mrtd.h"

EXPORT int mrtd_sub(int a, int b) { return a - b; }

EXPORT long long mrtd_wide(long long a, int b, short c) { return a * b + c; }

EXPORT int mrtd_apply(binary_fn f, int a, int b) {
  int total = 0;
  for (int i = 0; i < 1000; i++)
    total += f(a + i, b);
  return total;
}

EXPORT int mrtd_sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  int total = 0;
  for (int i = 0; i < n; i++)
    total += va_arg(ap, int);
  va_end(ap);
  return total;
}

EXPORT struct pair mrtd_make(int a, int b) {
  struct pair p = {a * 3, b - 1};
  return p;
}

EXPORT double mrtd_mix(float f, double d, int i) { return f * d + i; }
