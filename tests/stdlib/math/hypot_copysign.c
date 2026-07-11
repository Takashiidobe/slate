#include <math.h>
#include <stdio.h>
int main(void) {
  double x = -3.0;
  double y = 4.0;
  printf("%.6f %.6f %.6f\n", hypot(x, y), copysign(x, y), copysign(y, x));
  return 0;
}
