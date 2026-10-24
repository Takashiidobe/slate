#include <stdio.h>

#include "calls.h"

long long VC driver_vc(int a, double b, long long c, float d, int e, double f,
                       double g, double h) {
  return a * 10000000LL + (long long)(b * 10) * 1000000 + c * 100000 +
         (long long)(d * 10) * 1000 + e * 100 + (long long)(f * 10) +
         (long long)(g + h);
}

static double VC driver_callback(int a, double b, float c, long long d,
                                 double e, float f, double g, int h, double i) {
  return a * 1000000.0 + b * 100000 + c * 10000 + d * 1000 + e * 100 + f * 10 +
         g + h * 0.1 + i * 0.01;
}

int main(void) {
  printf("mix %.3f\n", vc_mix(1, 2.5, 3.25f, 4, 5.5, 6.75f, 7.0, 8, 9.125));
  struct pair swapped = vc_swap((struct pair){10, 20}, 3);
  printf("swap %d %d\n", swapped.a, swapped.b);
  printf("invoke %.2f\n", vc_invoke(driver_callback, 5));
  printf("driver %lld\n", call_driver(7));
  return 0;
}
