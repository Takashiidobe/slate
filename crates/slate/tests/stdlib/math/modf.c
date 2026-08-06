#include <math.h>
#include <stdio.h>
int main(void) {
  double ip         = 0.0;
  double volatile x = 3.75;
  double fp         = modf(x, &ip);
  printf("%f %f\n", ip, fp);
  return 0;
}
