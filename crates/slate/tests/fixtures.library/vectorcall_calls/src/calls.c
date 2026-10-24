#include "calls.h"

double VC vc_mix(int a, double b, float c, long long d, double e, float f,
                 double g, int h, double i) {
  return a * 100000.0 + b * 10000 + c * 1000 + d * 100 + e * 10 + f + g * 0.1 +
         h * 0.01 + i * 0.001;
}

struct pair VC vc_swap(struct pair p, int bias) {
  struct pair swapped = {p.b + bias, p.a - bias};
  return swapped;
}

double VC vc_invoke(vc_callback cb, int seed) {
  return cb(seed, 0.5, 1.5f, seed * 2LL, 2.5, 3.5f, 4.5, seed * 3, 5.5);
}

static double VC local_callback(int a, double b, float c, long long d, double e,
                                float f, double g, int h, double i) {
  return a + b + c + d + e + f + g + h + i;
}

long long call_driver(int seed) {
  return driver_vc(seed, 1.5, 2, 2.5f, 3, 3.5, 4.5, 5.5) * 3 +
         (long long)vc_invoke(local_callback, seed);
}
