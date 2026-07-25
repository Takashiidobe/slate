#include <stdio.h>

int main(void) {
  volatile double a = 0.5;
  volatile double b = 2.0;
  volatile double c = 8.0;
  volatile double d = 5.75;
  volatile double e = 2.0;
  volatile double f = 2.5;
  double trig = __builtin_sin(a) + __builtin_cos(a) + __builtin_tan(a);
  double logs = __builtin_log(c) + __builtin_log10(100.0) + __builtin_log2(c);
  double powers = __builtin_pow(b, 3.0) + __builtin_sqrt(c) +
                  __builtin_exp(1.0) + __builtin_exp2(3.0);
  double rem = __builtin_fmod(d, e);
  long rounded = __builtin_lround(f);
  long long rounded_ll = __builtin_llround(f);
  printf("%.3f %.3f %.3f %.3f %ld %lld\n", trig, logs, powers, rem, rounded,
         rounded_ll);
  return 0;
}
