#include <math.h>
#include <stdio.h>
int main(void) {
  double x = 0.5;
  double y = 1.0;
  printf("%.6f %.6f %.6f %.6f %.6f\n", tan(x), atan(x), atan2(y, x), asin(x),
         acos(x));
  return 0;
}
