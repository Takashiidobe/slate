#include <stdio.h>

int main(void) {
  double fma_v    = __builtin_fma(2.0, 3.0, 1.0);
  double hypot_v  = __builtin_hypot(3.0, 4.0);
  double fdim_v   = __builtin_fdim(5.0, 2.0);
  double fdim_neg = __builtin_fdim(2.0, 5.0);
  double cbrt_v   = __builtin_cbrt(27.0);
  double ldexp_v  = __builtin_ldexp(1.0, 4);
  double scalbn_v = __builtin_scalbn(10.0, 3);
  double logb_v   = __builtin_logb(10.0);
  int    ilogb_v  = __builtin_ilogb(10.0);

  printf("%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n", fma_v, hypot_v, fdim_v,
         fdim_neg, cbrt_v, ldexp_v, scalbn_v, logb_v, ilogb_v);
  return 0;
}
