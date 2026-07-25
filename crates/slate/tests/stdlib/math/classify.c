#include <math.h>
#include <stdio.h>
int main(void) {
  double zero = 0.0;
  double neg = -0.0;
  double finite = 3.5;
  double inf = INFINITY;
  double nan = NAN;
  printf("%d %d %d %d\n", isnan(nan), isinf(inf), isfinite(finite),
         signbit(neg));
  printf("%d %d %d %d %d\n", fpclassify(nan), fpclassify(inf), fpclassify(zero),
         fpclassify(finite), fpclassify(1.0e-320));
  return 0;
}
