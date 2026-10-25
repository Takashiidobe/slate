/* { dg-options "-mfpmath=387" } */
#include <stdio.h>
#include <float.h>

int main(void) {
  volatile double huge = 1e308;
  double scaled = (huge * 10) / 10;
  volatile float f = 16777216.0f;
  float g = (f + 1.0f) - f;
  printf("%d %g %g\n", FLT_EVAL_METHOD, scaled, (double)g);
  return 0;
}
