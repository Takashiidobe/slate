#include <math.h>
#include <stdio.h>
int main(void) {
  double x = 2.3;
  double y = -2.7;
  printf("%.6f %.6f %.6f %.6f\n", nearbyint(x), rint(y), fmin(x, y),
         fmax(x, y));
  printf("%.6f %.6f\n", fdim(x, y), fdim(y, x));
  return 0;
}
