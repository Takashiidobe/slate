#include "calls.h"

long MS_ABI ms_mix(int a, double b, long c, float d, long e, double f) {
  return a * 100000L + (long)(b * 10) * 1000 + c * 100 + (long)(d * 10) + e * 7 +
         (long)f;
}

struct pair MS_ABI ms_swap(struct pair p, int bias) {
  struct pair swapped = {p.b + bias, p.a - bias};
  return swapped;
}

struct triple MS_ABI ms_scale(struct triple t, long scale) {
  struct triple scaled = {t.x * scale, t.y * scale, t.z * scale};
  return scaled;
}

long MS_ABI ms_invoke(ms_callback cb, long seed) {
  return cb(seed, 0.5, seed * 2, 1.5, seed * 3);
}

long SYSV_ABI sysv_add(long a, long b) { return a + b; }

static long MS_ABI local_callback(long a, double b, long c, double d, long e) {
  return a * 10000 + (long)(b * 10) * 1000 + c * 100 + (long)(d * 10) + e;
}

long call_driver(long seed) {
  ms_callback cb = local_callback;
  return driver_ms(seed, 1.5, 2, 2.5, 3) * 3 + ms_invoke(cb, seed) +
         driver_vmix(2, seed, 2.5) + sysv_add(seed, 1);
}

long ms_list_sum(int n, __builtin_ms_va_list ap) {
  __builtin_ms_va_list again;
  __builtin_ms_va_copy(again, ap);
  long first = __builtin_va_arg(ap, long);
  long repeated = __builtin_va_arg(again, long);
  __builtin_ms_va_end(again);
  double d = __builtin_va_arg(ap, double);
  struct triple t = __builtin_va_arg(ap, struct triple);
  struct tiny small = __builtin_va_arg(ap, struct tiny);
  int last = __builtin_va_arg(ap, int);
  return n * 1000000L + first * 100000 + repeated * 10000 + (long)(d * 10) * 100 +
         t.x + t.y * 2 + t.z * 3 + small.a + small.b * 2 + small.c * 3 + last * 7;
}
