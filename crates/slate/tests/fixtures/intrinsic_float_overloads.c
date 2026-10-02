#include <stdio.h>

int main(void) {
  float small = -2.5f;
  double large = -3.75;
  long double extended = -4.125L;
  printf("%.3f %.3f %.3f %.3f\n", (double)__builtin_fabsf(small),
         __builtin_fabs(large), (double)__builtin_fabsl(extended),
         (double)__builtin_copysignl(0.0L, extended));
  return 0;
}
