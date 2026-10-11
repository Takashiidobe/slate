#include <stdio.h>

#include "calls.h"

long MS_ABI driver_ms(long a, double b, long c, double d, long e) {
  return a * 10000 + (long)(b * 10) * 1000 + c * 100 + (long)(d * 10) + e;
}

long MS_ABI driver_vmix(int n, ...) {
  __builtin_ms_va_list ap;
  __builtin_ms_va_start(ap, n);
  long l = __builtin_va_arg(ap, long);
  double d = __builtin_va_arg(ap, double);
  __builtin_ms_va_end(ap);
  return n * 1000 + l * 10 + (long)(d * 10);
}

static long MS_ABI driver_callback(long a, double b, long c, double d, long e) {
  return a + (long)(b * 100) + c * 1000 + (long)(d * 10000) + e * 100000;
}

int main(void) {
  printf("mix %ld\n", ms_mix(3, 4.5, 6, 7.5f, 8, 9.25));
  struct pair swapped = ms_swap((struct pair){1.25, 2.5}, 3);
  printf("swap %g %g\n", swapped.a, swapped.b);
  struct triple scaled = ms_scale((struct triple){1, 2, 3}, 4);
  printf("scale %ld %ld %ld\n", scaled.x, scaled.y, scaled.z);
  printf("invoke %ld\n", ms_invoke(driver_callback, 5));
  printf("sysv %ld\n", sysv_add(40, 2));
  printf("driver %ld\n", call_driver(7));
  return 0;
}
