/* { dg-options "-fcomplex-arithmetic=basic" } */
#include <stdio.h>
#include <math.h>

int main(void) {
  volatile double big = 1e300;
  double _Complex z = big + big * 1.0i;
  double _Complex q = z / z;
  double _Complex small = (1.0 + 2.0i) / (3.0 + 4.0i);
  printf("%d %d %.6f %.6f\n", isnan(__real__ q) || isinf(__real__ q), isnan(__imag__ q) || isinf(__imag__ q),
         __real__ small, __imag__ small);
  return 0;
}
