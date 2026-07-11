#include <math.h>
#include <stdio.h>
int main(void) {
  long double x = -3.75L;
  long double y = 2.5L;
  printf("%.6f %.6f %.6f %.6f %.6f\n", (double)fabsl(x), (double)ceill(x), (double)floorl(x), (double)roundl(y), (double)truncl(x));
  return 0;
}
