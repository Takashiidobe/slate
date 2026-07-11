#include <math.h>
#include <stdio.h>
int main(void) {
  double x = 1.5;
  double y = 8.0;
  printf("%.6f %.6f\n", ldexp(x, 4), scalbn(x, -2));
  printf("%d %.6f %.6f\n", ilogb(y), logb(y), logb(0.125));
  return 0;
}
