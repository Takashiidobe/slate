/* { dg-options "-ffp-model=strict" } */
#include <fenv.h>
#include <stdio.h>

int main(void) {
  fesetround(FE_UPWARD);
  double third = 1.0 / 3.0;
  fesetround(FE_TONEAREST);
  volatile double a = 0x1.0000001p0, b = 0x1.0000001p0, c = -1.0;
  double fused = a * b + c;
  printf("%a %a\n", third, fused);
  return 0;
}
